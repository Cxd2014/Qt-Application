#include "widget.h"
#include <QMenu>
#include <QContextMenuEvent>

//创建主界面

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("音乐播放器"));
    setMaximumWidth(320);
    setMinimumWidth(320);
    setMinimumHeight(500);

    CreateButton();
    CreatePlayList();
    CreateMusicPlayer();

    //MusicFilePath="c:";
}

void Widget::CreateMusicPlayer()
{
    MainLayout = new QGridLayout;

    //创建播放音乐器
    mediaObject = new Phonon::MediaObject(this);
    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(mediaObject, audioOutput);
    //创建控制播放进度的滑块
    seekSlider = new Phonon::SeekSlider(mediaObject, this);
    //音量控制部件
    volumeSlider = new Phonon::VolumeSlider(audioOutput, this);
    volumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    //单击获取选中的歌曲行号
    connect(PlayList,SIGNAL(cellClicked(int,int)),this,SLOT(ChangRow(int)));
    //若双击了播放列表里的歌曲就播放该歌曲
    connect(PlayList, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableClicked(int)));
    //创建用来解析媒体的信息的元信息解析器
    metaInformationResolver = new Phonon::MediaObject(this);
    //需要与AudioOutput；连接后才能使用metaInformationResolver
    Phonon::AudioOutput *metaInformationAudioPutput =
            new Phonon::AudioOutput(Phonon::MusicCategory,this);
    Phonon::createPath(metaInformationResolver,metaInformationAudioPutput);
    //歌曲改变后把状态传给metaStateChanged函数
    connect(metaInformationResolver, SIGNAL(stateChanged(Phonon::State, Phonon::State)),
            this, SLOT(metaStateChanged(Phonon::State)));

    //歌曲播放源发生改变时传给SourceChanged函数
    connect(mediaObject,SIGNAL(currentSourceChanged(Phonon::MediaSource)),this,
            SLOT(SourceChanged(Phonon::MediaSource)));
    //当歌曲播放完后传给aboutTofinish函数播放下一首歌
    connect(mediaObject,SIGNAL(aboutToFinish()),this,SLOT(aboutToFinish()));
    //获取下一首歌曲的名字
    connect(mediaObject,SIGNAL(tick(qint64)),this,SLOT(GetNextMusicName()));
    //关联媒体对象的tick()信号来更新播放时间的显示
    mediaObject->setTickInterval(1000);//设置信号发射时间间隔
    //媒体对象mediaObject后在指定的时间间隔发射tick信号，信号的值是音乐当时播放的时间
    connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(updateTime(qint64)));

    //获取歌曲播放的状态用来改变歌曲名字
    connect(mediaObject,SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this,SLOT(GetMusicName(Phonon::State)));

    //控制界面布局
    MainLayout->addWidget(TitleLabel,0,0);
    MainLayout->addWidget(seekSlider,1,0,1,2);
    MainLayout->addLayout(LabelLayout,2,0,1,2);
    MainLayout->addLayout(ButtonLayout,3,0);
    MainLayout->addWidget(volumeSlider,3,1);
    MainLayout->setSizeConstraint(QLayout::SetFixedSize);//用户无法改变窗口的大小，保持窗口相对固定
    //总体布局
    Layout = new QVBoxLayout(this);
    Layout->addLayout(MainLayout);
    Layout->addWidget(PlayList);
}

// 根据媒体源列表内容和当前媒体源的位置来改变主界面控制按钮的状态
void Widget::changeActionState()
{
    // 如果媒体源列表为空
    if (sources.count() == 0)
    { 
        StartButton->setEnabled(false);
        StopButton->setEnabled(false);
        NextButton->setEnabled(false);
        LastButton->setEnabled(false);
    }
    else
    { // 如果媒体源列表不为空
        StartButton->setEnabled(true);
        StopButton->setEnabled(true);
        // 如果媒体源列表只有一行
        if (sources.count() == 1)
        {
            NextButton->setEnabled(false);
            LastButton->setEnabled(false);
        }
        else
        { // 如果媒体源列表有多行
            NextButton->setEnabled(true);
            LastButton->setEnabled(true);
            int index = PlayList->currentRow();
            // 如果播放列表当前选中的行为第一行
            if (index == 0)
                LastButton->setEnabled(false);
            // 如果播放列表当前选中的行为最后一行
            if (index + 1 == sources.count())
                NextButton->setEnabled(false);
        }
    }
}

void Widget::CreatePlayList()
{
     PlayList = new QTableWidget;
     // 设置行列数目
     PlayList->setRowCount(0);
     PlayList->setColumnCount(3);
     // 设置表头标签
     QStringList list;
     list << tr("歌曲") << tr("艺术家") << tr("时间");
     PlayList->setHorizontalHeaderLabels(list);
     //设置只能选择单行
     PlayList->setSelectionMode(QAbstractItemView::SingleSelection);
     PlayList->setSelectionBehavior(QAbstractItemView::SelectRows);
     // 设置不显示网格
     PlayList->setShowGrid(false);
}

void Widget::CreateButton()
{
    ButtonLayout = new QHBoxLayout;

    QSize iconSize(40,40);//设置图标大小
    StartButton = new QToolButton;
    StartButton->setIcon(QPixmap(":/image/3.png"));
    StartButton->setAutoRaise(true);//鼠标放在按钮上面才显示按钮阴影框
    StartButton->setIconSize(iconSize);
    connect(StartButton,SIGNAL(clicked()),this,SLOT(StartMusic()));

    StopButton = new QToolButton;
    StopButton->setIcon(QPixmap(":/image/2.png"));
    StopButton->setVisible(false);//隐藏按钮
    StopButton->setAutoRaise(true);
    StopButton->setIconSize(iconSize);
    connect(StopButton,SIGNAL(clicked()),this,SLOT(StopMusic()));

    NextButton = new QToolButton;
    NextButton->setIcon(QPixmap(":/image/1.png"));
    NextButton->setAutoRaise(true);
    NextButton->setIconSize(iconSize);
    connect(NextButton,SIGNAL(clicked()),this,SLOT(NextMusic()));

    LastButton = new QToolButton;
    LastButton->setIcon(QPixmap(":/image/4.png"));
    LastButton->setAutoRaise(true);
    LastButton->setIconSize(iconSize);
    connect(LastButton,SIGNAL(clicked()),this,SLOT(LastMusic()));

    OpenButton = new QToolButton;
    OpenButton->setIcon(QPixmap(":/image/6.png"));
    OpenButton->setAutoRaise(true);
    OpenButton->setIconSize(iconSize);
    connect(OpenButton,SIGNAL(clicked()),this,SLOT(OpenFile()));

    ButtonLayout->addWidget(LastButton,0,0);
    ButtonLayout->addWidget(StartButton,1,0);
    ButtonLayout->addWidget(StopButton,1,0);
    ButtonLayout->addWidget(NextButton,2,0);
    ButtonLayout->addWidget(OpenButton,3,0);

    LabelLayout = new QHBoxLayout;

    timeLabel= new QLabel;//显示播放时间
    timeLabel->setText("00:00");
    TotaltimeLabel= new QLabel;//显示歌曲时间长度
    TotaltimeLabel->setText("00:00");

    TitleLabel= new QLabel;//显示歌曲名字
    TitleLabel->setText(tr("Music Title"));

    LabelLayout->addWidget(timeLabel);
    LabelLayout->addStretch();//中间空开
    LabelLayout->addWidget(TotaltimeLabel);
}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(tr("删除"),this, SLOT(DelateMusic()));
    menu.addAction(tr("清空列表"), this, SLOT(ClearPlayList()));
    menu.exec(event->globalPos());
}

Widget::~Widget()
{
    
}
