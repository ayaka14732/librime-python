# -*- coding: utf-8 -*-

from typing import Optional

class TranslatorAnswer:
    '''
    The answer to a translator query.
    '''

    def __init__(self, text: Optional[str] = None, length: Optional[int] = None, candidate_type: Optional[str] = None, comment: Optional[str] = None, preedit: Optional[str] = None):
        self.text = text
        self.length = length
        self.candidate_type = candidate_type
        self.comment = comment
        self.preedit = preedit

    def __repr__(self) -> str:
        return f'<Candidate object; text={repr(self.text)}, length={self.length}>'

class TranslatorQuery:
    '''
    The input to the translator.
    '''

    def __init__(self, text: str):
        self.text = text

    def __repr__(self) -> str:
        return f'<TranslatorQuery object; text={repr(self.text)}>'

    def create_answer(self, text: str, length: Optional[int] = None, candidate_type: Optional[str] = None, comment: Optional[str] = None, preedit: Optional[str] = None) -> TranslatorAnswer:
        # default values
        if length is None:
            length = len(self.text)
        if candidate_type is None:
            candidate_type = 'python'
        if comment is None:
            comment = ''
        if preedit is None:
            preedit = ''

        # type checking
        assert isinstance(text, str)
        assert isinstance(length, int)
        assert isinstance(candidate_type, str)
        assert isinstance(comment, str)
        assert isinstance(preedit, str)

        return TranslatorAnswer(text=text, length=length, candidate_type=candidate_type, comment=comment, preedit=preedit)
