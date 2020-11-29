#include "transform.h"
#include <QtCore>
QStringList CompileStates = {"CompileSuccessfully",
                             "NoValidSourceFile",
                             "CompileError",
                             "CompileTimeLimitExceeded",
                             "InvalidCompiler"};
QStringList ResultStates = {"CorrectAnswer",
                            "WrongAnswer",
                            "PartlyCorrect",
                            "TimeLimitExceeded",
                            "MemoryLimitExceeded",
                            "CannotStartProgram",
                            "FileError",
                            "RunTimeError",
                            "InvalidSpecialJudge",
                            "SpecialJudgeTimeLimitExceeded",
                            "SpecialJudgeRunTimeError",
                            "Skipped",
                            "InteractorError",
                            "PresentationError",
                            "OutputLimitExceeded"};
Transform::Transform()
{
}
#define W(j, x) j[#x] = x
#define L(x) #x;
void Transform::doit(const QString &inFilePath, const QString &outFilePath)
{
    QFile inFile(inFilePath);
    QFile outFile(outFilePath);
    inFile.open(QFile::OpenModeFlag::ReadOnly);
    outFile.open(QFile::OpenModeFlag::WriteOnly);
    QDataStream _in(&inFile);
    unsigned checkNumber = 0;
    _in >> checkNumber;
    quint16 checksum = 0;
    int len = 0;
    _in >> checksum >> len;
    char *raw = new char[len];
    _in.readRawData(raw, len);
    QByteArray data(raw, len);
    delete[] raw;
    data = qUncompress(data);
    QDataStream in(data);
    QJsonObject out;
    QString contestTitle;
    int count = 0;
    in >> contestTitle;
    W(out, contestTitle);
    in >> count;
    QJsonArray tasks;
    for (int i = 0; i < count; i++)
    {
        QJsonObject task;
        int tmp = 0;
        int count = 0;
        QString problemTitle, sourceFileName, inputFileName, outputFileName;
        bool standardInputCheck, standardOutputCheck;
        in >> problemTitle;
        in >> sourceFileName;
        in >> inputFileName;
        in >> outputFileName;
        in >> standardInputCheck;
        in >> standardOutputCheck;
        W(task, problemTitle);
        W(task, sourceFileName);
        W(task, inputFileName);
        W(task, outputFileName);
        W(task, standardInputCheck);
        W(task, standardOutputCheck);
        in >> tmp;
        int taskType = tmp & 0xFF;
        int subFolderCheck = tmp >> 8;
        int comparisonMode;
        in >> comparisonMode;
        W(task, taskType);
        W(task, subFolderCheck);
        W(task, comparisonMode);
        QString diffArguments, specialJudgePath;
        int realPrecision;
        in >> diffArguments;
        in >> realPrecision;
        in >> specialJudgePath;
        specialJudgePath.replace('/', QDir::separator());
        W(task, diffArguments);
        W(task, realPrecision);
        W(task, specialJudgePath);
        QMap<QString, QString> tcompilerConfiguration;
        QString answerFileExtension;
        in >> tcompilerConfiguration;
        in >> answerFileExtension;
        in >> count;
        QJsonObject compilerConfiguration;
        for (auto [x, y] : tcompilerConfiguration.toStdMap())
        {
            compilerConfiguration[x] = y;
        }
        W(task, compilerConfiguration);
        W(task, answerFileExtension);
        QJsonArray testcases;
        for (int i = 0; i < count; i++)
        {
            QJsonObject testcase;
            int fullScore, timeLimit, memoryLimit;
            in >> fullScore;
            in >> timeLimit;
            in >> memoryLimit;
            QStringList t_inputFiles, toutputFiles;
            in >> t_inputFiles;
            in >> toutputFiles;
            W(testcase, fullScore);
            W(testcase, timeLimit);
            W(testcase, memoryLimit);
            QJsonArray _inputFiles, outputFiles;
            for (auto i : t_inputFiles)
                _inputFiles.append(i);
            for (auto i : toutputFiles)
                outputFiles.append(i);
            W(testcase, _inputFiles);
            W(testcase, outputFiles);
            testcases.append(testcase);
        }
        W(task, testcases);
        tasks.append(task);
    }
    W(out, tasks);
    in >> count;
    QJsonArray contestants;
    for (int i = 0; i < count; i++)
    {
        QJsonObject contestant;
        QString contestantName;
        QList<bool> _checkJudged;
        QStringList _sourceFiles, _compileMesaage;
        QList<QList<QStringList>> _inputFiles, _message;
        QList<QList<QList<int>>> _score, _timeUsed, _memoryUsed;
        in >> contestantName;
        in >> _checkJudged;
        in >> _sourceFiles;
        in >> _compileMesaage;
        in >> _inputFiles;
        in >> _message;
        in >> _score;
        in >> _timeUsed;
        in >> _memoryUsed;
        quint32 judgingTime_date = 0;
        quint32 judgingTime_time = 0;
        quint8 judgingTime_timespec = 0;
        in >> judgingTime_date;
        in >> judgingTime_time;
        in >> judgingTime_timespec;
        QDateTime judgingTime = QDateTime(QDate::fromJulianDay(judgingTime_date),
                                          QTime::fromMSecsSinceStartOfDay(static_cast<int>(judgingTime_time)),
                                          Qt::TimeSpec(judgingTime_timespec));
        W(contestant, contestantName);
        QJsonArray checkJudged;
        for (auto i : _checkJudged)
            checkJudged.append(i);
        W(contestant, checkJudged);
        auto f = [](const auto &shit) {
            QJsonArray a;
            for (const auto &i : shit)
            {
                QJsonArray b;
                for (const auto &j : i)
                {
                    QJsonArray c;
                    for (const auto &k : j)
                    {
                        c.append(k);
                    }
                    b.append(c);
                }
                a.append(b);
            }
            return a;
        };
        auto f2 = [](const QStringList &shit) {
            QJsonArray a;
            for (auto i : shit)
            {
                a.append(i);
            }
            return a;
        };
        auto sourceFiles = f2(_sourceFiles);
        auto compileMesaage = f2(_compileMesaage);
        auto inputFiles = f(_inputFiles);
        auto message = f(_message);
        auto score = f(_score);
        auto memoryUsed = f(_memoryUsed);
        auto timeUsed = f(_timeUsed);
        W(contestant, sourceFiles);
        W(contestant, compileMesaage);
        W(contestant, inputFiles);
        W(contestant, message);
        W(contestant, score);
        W(contestant, memoryUsed);
        W(contestant, timeUsed);
        contestant["judgingTime"] = judgingTime.toString();
        int count = 0;
        int _count = 0;
        int __count = 0;
        int tmp = 0;
        in >> count;
        QJsonArray compileState;
        for (int i = 0; i < count; i++)
        {
            in >> tmp;
            compileState.append(CompileStates[tmp]);
        }
        W(contestant, compileState);
        in >> count;
        QJsonArray result;
        for (int i = 0; i < count; i++)
        {
            QJsonArray resultA;
            in >> _count;
            for (int j = 0; j < _count; j++)
            {
                QJsonArray resultB;
                in >> __count;
                for (int k = 0; k < __count; k++)
                {
                    in >> tmp;
                    resultB.append(ResultStates[tmp]);
                }
                resultA.append(resultB);
            }
            result.append(resultA);
        }
        W(contestant, result);
        contestants.append(contestant);
    }
    W(out, contestants);
    outFile.write(QJsonDocument(out).toJson());
}
