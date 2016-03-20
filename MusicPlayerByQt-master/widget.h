#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <Phonon>
#include <QLabel>
#include <QToolButton>
#include <QLayout>
#include <QString>
#include <QTime>
#include <QMessageBox>
#include <QFileInfo>
#include <QTableWidget>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMenu>
#include <QContextMenuEvent>
#include <QList>

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    Widget(QWidget *parent = 0);
    ~Widget();

    int row;//存放播放列表中选中的行
    //QString MusicFilePath;//用来保存文件路径
private:
    //重新定义鼠标右击时间,使单击右键出现菜单栏
    void contextMenuEvent(QContextMenuEvent *event);

private slots:

    void ChangRow(int R);//改变选中的行数
    void StopMusic();//停止音乐播放
    void StartMusic();//开始音乐播放
    void NextMusic();//上一首歌曲
    void LastMusic();//下一首歌曲
    void OpenFile();//打开音乐文件
    void updateTime(qint64 time);//更新显示歌曲时间
    void GetMusicName(Phonon::State newState);//获取歌曲的名字并判断播放状态是否正确
    void GetNextMusicname();//获取下一首歌的歌名
    void ClearPlayList();//清空播放列表
    void DelateMusic();//从播放列表中删除的选中歌曲

    //当媒体源改变时，在播放列表中选中相应的行并更新图标的状态并获取歌曲名字
    void SourceChanged(const Phonon::MediaSource &source);
    void aboutToFinish();//当一首个播放完后指定播放下一首歌

    //解析媒体文件的元信息获取歌曲信息及将歌曲插入到播放列表
    void metaStateChanged(Phonon::State newState);
    void tableClicked(int row);//若双击了播放列表里的歌曲就播放该歌曲

private:
    void CreateButton();//创建按钮
    void CreateMusicPlayer();//创建音乐播放器
    void CreatePlayList();//创建播放歌曲列表
    void changeActionState();//根据媒体源列表内容和当前媒体源的位置来改变主界面控制按钮的状态

    Phonon::MediaObject *mediaObject;//创建播放音乐器
    Phonon::SeekSlider *seekSlider;//创建控制播放进度的滑块
    Phonon::VolumeSlider *volumeSlider;//音量控制部件

    QTableWidget *PlayList;//创建歌曲列表
    QList<Phonon::MediaSource> sources;//创建一个播放源
    //创建用来解析媒体的信息的元信息解析器
    Phonon::MediaObject *metaInformationResolver;

    QLabel *TitleLabel;//显示歌曲名字
    QLabel *timeLabel;//显示播放时间
    QLabel *TotaltimeLabel;//显示歌曲时间长度

    QToolButton *StartButton;
    QToolButton *StopButton;
    QToolButton *NextButton;
    QToolButton *LastButton;
    QToolButton *OpenButton;

    QGridLayout *MainLayout;//布局管理
    QVBoxLayout *Layout;
    QHBoxLayout *ButtonLayout;
    QHBoxLayout *LabelLayout;

};

#endif // WIDGET_H
