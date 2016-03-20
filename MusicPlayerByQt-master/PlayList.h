#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QTableWidget>

class PlayList:public QTableWidget
{
    Q_OBJECT
public:
    explicit PlayList(QWidget *parent=0);

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void closeEvent(QCloseEvent *event);
signals:
    void playlistClean();

private slots:
    void clearPlaylist();
};

#endif // PLAYLIST_H
