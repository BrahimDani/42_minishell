#!/bin/bash
# Script pour lancer valgrind avec tous les flags nécessaires
# et ignorer les leaks de readline

valgrind \
  --leak-check=full \
  --show-leak-kinds=all \
  --track-origins=yes \
  --track-fds=yes \
  --verbose \
  --suppressions=readline.supp \
  --log-file=valgrind_output.log \
  ./minishell
