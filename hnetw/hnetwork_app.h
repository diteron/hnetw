#pragma once

class HnetworkApp : public QApplication {
public:
    HnetworkApp(int& argc, char** argv);
    ~HnetworkApp();

private:
    void setSystemColorScheme();
};

