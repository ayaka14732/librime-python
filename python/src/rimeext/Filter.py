# -*- coding: utf-8 -*-

from typing import Optional

class FilterAnswer:
    '''
    The answer to a filter query.
    '''

    def __init__(self, should_skip: bool = False, should_remove: bool = False, candidate_type: Optional[str] = None, text: Optional[str] = None, comment: Optional[str] = None, direct_call: bool = True):
        assert not direct_call, 'Please use `Skip`, `Remove` or `Rewrite` instead of calling this constructor directly'

        self.should_skip = should_skip

        self.should_remove = should_remove

        self.candidate_type = candidate_type
        self.text = text
        self.comment = comment

    @classmethod
    def Skip(cls):
        return cls(should_skip=True, direct_call=False)

    @classmethod
    def Remove(cls):
        return cls(should_remove=True, direct_call=False)

    @classmethod
    def Rewrite(cls, **kwargs):
        return cls(**kwargs, direct_call=False)

class FilterQuery:
    '''
    The input to the filter.
    '''

    def __init__(self, candidate_type: str, text: str, comment: str, preedit: str):
        self.candidate_type = candidate_type
        self.text = text
        self.comment = comment
        self.preedit = preedit

    def __repr__(self) -> str:
        return f'<FilterQuery object; text={repr(self.text)}>'

    def do_skip(self) -> FilterAnswer:
        '''Skip the current candidate. The candidate will be remained intact.'''
        return FilterAnswer.Skip()

    def do_remove(self) -> FilterAnswer:
        '''Remove the current candidate from the candidate list.'''
        return FilterAnswer.Remove()

    def do_rewrite(self, candidate_type: Optional[str] = None, text: Optional[str] = None, comment: Optional[str] = None) -> FilterAnswer:
        '''Change certain attributes of the current candidate.'''
        # default values
        if candidate_type is None:
            candidate_type = 'python_filtered'
        if text is None:
            text = self.text
        if comment is None:
            comment = self.comment

        # type checking
        assert isinstance(candidate_type, str)
        assert isinstance(text, str)
        assert isinstance(comment, str)

        return FilterAnswer.Rewrite(text=text, candidate_type=candidate_type, comment=comment)
