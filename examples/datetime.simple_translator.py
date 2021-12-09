# -*- coding: utf-8 -*-
# 输入 date 时输出当前日期

from datetime import datetime
from typing import List, Optional

def rime_main(s: str) -> Optional[List[str]]:
    if s == 'date':
        date = datetime.now().strftime('%Y-%m-%d')
        return [date]
