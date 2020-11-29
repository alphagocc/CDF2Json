#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <QStringList>
enum CompileState
{
    CompileSuccessfully,
    NoValidSourceFile,
    CompileError,
    CompileTimeLimitExceeded,
    InvalidCompiler
};

enum ResultState
{
    CorrectAnswer,
    WrongAnswer,
    PartlyCorrect,
    TimeLimitExceeded,
    MemoryLimitExceeded,
    CannotStartProgram,
    FileError,
    RunTimeError,
    InvalidSpecialJudge,
    SpecialJudgeTimeLimitExceeded,
    SpecialJudgeRunTimeError,
    Skipped,
    InteractorError,
    PresentationError,
    OutputLimitExceeded
};

class Transform
{
public:
    Transform();
    void doit(const QString &, const QString &);
};

#endif // TRANSFORM_H
