// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtWidgets/QApplication>

#include "common/config.h"
#include "core/file_sys/fs.h"
#include "qt_gui/game_install_dialog.h"
#include "qt_gui/main_window.h"

void customMessageHandler(QtMsgType, const QMessageLogContext&, const QString&) {}

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    const auto config_dir = Common::FS::GetUserPath(Common::FS::PathType::UserDir);
    Config::load(config_dir / "config.toml");
#ifdef _WIN64
    QString gameDataPath = qApp->applicationDirPath() + "/game_data/";
    std::wstring wstdStr = gameDataPath.toStdWString();
    std::filesystem::path path(wstdStr);
#else
    std::string stdStr = std::string(config_dir) + "/game_data/";
    std::filesystem::path path(stdStr);
#endif
    std::filesystem::create_directory(path);

    if (Config::getGameInstallDir() == "") {
        GameInstallDialog dlg;
        dlg.exec();
    }
    qInstallMessageHandler(customMessageHandler); // ignore qt logs.

    MainWindow* m_main_window = new MainWindow(nullptr);
    m_main_window->Init();

    return a.exec();
}
