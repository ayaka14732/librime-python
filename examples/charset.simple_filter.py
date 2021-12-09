# -*- coding: utf-8 -*-
# 过滤拓展区汉字

import re

# https://ayaka.shn.hk/hanregex/
# CJK Unified Ideographs Extension A to G
extblk = re.compile(r'[\u3400-\u4dbf\U00020000-\U0002a6df\U0002a700-\U0002ebef\U00030000-\U0003134f]')

def rime_main(s: str) -> bool:
    return not extblk.match(s)
