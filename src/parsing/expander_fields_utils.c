/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_fields_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 20:55:26 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 03:15:20 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

void	handle_single_field(t_token *cur, t_token *prev, char *word,
			t_split_meta *meta)
{
	char	*copy;

	copy = ft_strdup(word);
	if (!copy)
		return ;
	free(cur->value);
	cur->value = copy;
	if (prev && cur->space_before == 0 && meta->leading > 0)
		cur->space_before = 1;
	if (meta->trailing > 0 && cur->next && cur->next->space_before == 0)
		cur->next->space_before = 1;
}

t_token	*handle_multi_fields(t_token *cur, t_token *prev,
			char **split, t_split_meta *meta)
{
	int		idx;
	t_token	*last;

	free(cur->value);
	cur->value = ft_strdup(split[0]);
	if (prev && cur->space_before == 0 && meta->leading > 0)
		cur->space_before = 1;
	last = cur;
	idx = 1;
	while (split[idx])
	{
		last = append_field_token(last, split[idx]);
		if (!last)
			break ;
		idx++;
	}
	return (last);
}

static t_token	*handle_multi_split(t_field_ctx *ctx, char **split,
			t_split_meta *meta)
{
	t_token	*last;

	last = handle_multi_fields(ctx->cur, *(ctx->prev), split, meta);
	if (meta->trailing > 0 && last && last->next
		&& last->next->space_before == 0)
		last->next->space_before = 1;
	*(ctx->prev) = last;
	free_split_words(split);
	return (last->next);
}

t_token	*process_fields(t_field_ctx *ctx, char **split, t_split_meta *meta)
{
	int	count;

	count = count_fields(split);
	if (count == 0)
	{
		free_split_words(split);
		if (meta->len == 0)
			return (handle_empty_result(ctx->cur, ctx->head, *(ctx->prev)));
		return (handle_spaces_only(ctx->cur, ctx->head, *(ctx->prev)));
	}
	if (count == 1)
	{
		handle_single_field(ctx->cur, *(ctx->prev), split[0], meta);
		free_split_words(split);
	}
	else
		return (handle_multi_split(ctx, split, meta));
	*(ctx->prev) = ctx->cur;
	return (ctx->cur->next);
}
