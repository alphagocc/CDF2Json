#include <transform.h>
#include <QDebug>
int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    Transform t;
    qDebug() << argc;
    t.doit(argv[1], argv[2]);
    return 0;
}
