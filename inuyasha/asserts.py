from test_symbols import ERROR_TEST_ASSERT_FAILED


class Assert(object):

    @staticmethod
    def IsTrue(expression):
        if not expression:
            raise Exception(ERROR_TEST_ASSERT_FAILED)
        pass
    
    @staticmethod
    def IsFalse(expression):
        if expression:
            raise Exception(ERROR_TEST_ASSERT_FAILED)
        
        
