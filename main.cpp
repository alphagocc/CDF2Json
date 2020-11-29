#include <transform.h>
#include <QDebug>
int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    Transform t;
    qDebug() << argc;
    if (argc == 1)
    {
        t.doit("C:\\Users\\Alphagocc\\Desktop\\Code\\20170902p\\2017-09-02.cdf", "aaaaaa.json");
        return 0;
    }
    t.doit(argv[1], argv[2]);
    return 0;
}
