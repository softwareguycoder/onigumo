import base64
class BytesToBase64Translator(object):
    @staticmethod
    def Translate(the_bytes):
        if len(the_bytes) == 0:
            return ''
        return base64.b64encode(bytes(the_bytes)).decode('ascii')