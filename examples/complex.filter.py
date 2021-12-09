# -*- coding: utf-8 -*-
# 删除长度为 2 的候选，并将长度为 3 的候选转为简体

from opencc import OpenCC
from rimeext import FilterQuery, FilterAnswer

t2s = OpenCC('t2s').convert

def rime_main(filter_query: FilterQuery) -> FilterAnswer:
    text = filter_query.text
    comment = filter_query.comment
    if len(text) == 2:
        return filter_query.do_remove()
    if len(text) == 3:
        return filter_query.do_rewrite(text=t2s(text), comment=comment + ' [简]')
    return filter_query.do_skip()
