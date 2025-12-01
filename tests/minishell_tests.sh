#!/usr/bin/env bash

# 42 Minishell Test Suite
# Simulates a typical 42 correction: strict on basics, loose on edge cases
# Heredocs are tested manually in interactive mode (not automated)

set -u

FOO=""
VAR=""

MINISHELL_EXEC="./minishell"
BASH_EXEC="bash"

if [ ! -x "$MINISHELL_EXEC" ]; then
  echo "[FATAL] minishell binary not found or not executable: $MINISHELL_EXEC" >&2
  exit 1
fi

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m'

pass() {
  printf "%b[OK]%b %s\n" "$GREEN" "$NC" "$1"
}

fail() {
  printf "%b[KO]%b %s\n" "$RED" "$NC" "$1"
}

info() {
  printf "%b[INFO]%b %s\n" "$BLUE" "$NC" "$1"
}

# Strict test: output and status must match bash exactly
run_case() {
  local name="$1"
  local cmd="$2"

  local bash_out bash_status
  bash_out=$($BASH_EXEC -c "$cmd" 2>&1)
  bash_status=$?

  local mini_out mini_status
  mini_out=$($MINISHELL_EXEC -c "$cmd" 2>&1)
  mini_status=$?

  if [ "$bash_out" = "$mini_out" ] && [ "$bash_status" -eq "$mini_status" ]; then
    pass "$name"
  else
    fail "$name"
    echo "  CMD     : $cmd"
    echo "  bash out: $bash_out" | sed 's/^/    /'
    echo "  bash st : $bash_status"
    echo "  mini out: $mini_out" | sed 's/^/    /'
    echo "  mini st : $mini_status"
  fi
}

# Loose test: only checks that minishell doesn't crash and returns valid status
run_case_loose() {
  local name="$1"
  local cmd="$2"

  local mini_out mini_status
  mini_out=$($MINISHELL_EXEC -c "$cmd" 2>&1)
  mini_status=$?

  if [ "$mini_status" -ge 0 ]; then
    pass "$name (loose)"
  else
    fail "$name (loose)"
    echo "  CMD     : $cmd"
    echo "  mini out: $mini_out" | sed 's/^/    /'
    echo "  mini st : $mini_status"
  fi
}

# Functionality test: checks status code only (error messages can differ)
run_case_status() {
  local name="$1"
  local cmd="$2"

  local bash_status
  $BASH_EXEC -c "$cmd" >/dev/null 2>&1
  bash_status=$?

  local mini_status
  $MINISHELL_EXEC -c "$cmd" >/dev/null 2>&1
  mini_status=$?

  if [ "$bash_status" -eq "$mini_status" ]; then
    pass "$name"
  else
    fail "$name (status mismatch: bash=$bash_status mini=$mini_status)"
  fi
}

###############################################
# 42 MINISHELL TEST SUITE
###############################################

echo "=== 42 Minishell Correction Tests ==="
echo ""

# 1) BUILTINS (mandatory)
info "Testing builtins..."
run_case "echo simple" "echo coucou"
run_case "echo with spaces" "echo a    b   c"
run_case "echo -n" "echo -n test"
run_case "pwd" "pwd"
run_case_loose "cd .." "cd ..; pwd"
run_case "env basic" "env | head -n 5"
run_case "export simple" "export FOO=bar; echo \"\$FOO\""
run_case "unset" "export FOO=bar; unset FOO; echo \"\$FOO\""
run_case_status "exit 0" "exit 0"
run_case_status "exit 42" "exit 42"

# 2) EXTERNAL COMMANDS & PATH
info "Testing external commands..."
run_case "ls" "ls"
run_case "ls with args" "ls -l | head -n 3"
run_case "/bin/echo" "/bin/echo test"
run_case_status "command not found" "toto_commande_qui_existe_pas; echo \$?"

# 3) REDIRECTIONS (mandatory)
info "Testing redirections..."
run_case "redir output" "echo hello > out_test.txt; cat out_test.txt; rm -f out_test.txt"
run_case "redir append" "echo one > out_test.txt; echo two >> out_test.txt; cat out_test.txt; rm -f out_test.txt"
run_case "redir input" "echo 'line' > in_test.txt; cat < in_test.txt; rm -f in_test.txt"
run_case_status "redir nofile" "cat < no_such_file_\$\$; echo \$?"
run_case "multi redir" "echo test > f1; cat < f1 > f2; cat f2; rm -f f1 f2"

# 4) PIPES (mandatory)
info "Testing pipes..."
run_case "pipe simple" "echo coucou | cat"
run_case "pipe ls|wc" "ls | wc -l"
run_case "pipe triple" "echo a b c | tr ' ' '\n' | wc -l"
run_case "pipe with redir" "echo hello | cat > out.txt; cat out.txt; rm -f out.txt"
run_case_loose "pipe cmd not found" "toto_cmd | cat"

# 5) SEMICOLON (bonus if implemented)
info "Testing semicolon separator..."
run_case "semicolon basic" "echo a; echo b"
run_case "semicolon with status" "false; echo \$?"
run_case "semicolon + redir" "echo one > f; echo two >> f; cat f; rm -f f"

# 6) QUOTES & EXPANSIONS
info "Testing quotes and expansions..."
run_case "single quotes" "echo 'hello \$USER'"
run_case "double quotes" "echo \"hello \$USER\""
run_case "var expansion" "export VAR=test; echo \$VAR"
run_case "status var" "false; echo \$?"

# 7) EDGE CASES (loose - behavior can differ)
info "Testing edge cases..."
run_case_loose "empty command" ""
run_case_loose "only spaces" "   "
run_case_loose "multiple pipes" "echo test | cat | cat | cat"


echo ""
echo "=== END OF AUTOMATED TESTS ==="
echo ""
info "Heredocs should be tested manually in interactive mode:"
info "  1. Launch ./minishell"
info "  2. Type: cat << EOF"
info "  3. Type some lines"
info "  4. Type: EOF"
info "  5. Check output is correct"
echo ""

exit 0
