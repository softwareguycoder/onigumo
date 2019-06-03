from common.string_utils import StringUtilities
class ProcessInfoValidator(object):
    @staticmethod
    def IsValid(processInfo):
        return processInfo is not None \
            and processInfo.get_PID() > 0 \
            and not StringUtilities \
                .IsNullOrWhiteSpace(processInfo.get_TTY()) \
            and not StringUtilities \
                .IsNullOrWhiteSpace(processInfo.get_CPUTIME()) \
            and not StringUtilities \
                .IsNullOrWhiteSpace(processInfo.get_CMD())