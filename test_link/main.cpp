#include <iostream>
#include <string>
#include <QtUtil/QtUtilInfos.h>

int main() {
    QtUtilInfos qtutil_infos;
    std::cout << "Version " << qtutil_infos.getVersion() << std::endl;
    return 0;
}
