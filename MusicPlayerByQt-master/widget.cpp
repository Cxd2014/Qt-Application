#include "widget.h"
#include <QMenu>
#include <QContextMenuEvent>

//����������

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("���ֲ�����"));
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

    //��������������
    mediaObject = new Phonon::MediaObject(this);
    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(mediaObject, audioOutput);
    //�������Ʋ��Ž��ȵĻ���
    seekSlider = new Phonon::SeekSlider(mediaObject, this);
    //�������Ʋ���
    volumeSlider = new Phonon::VolumeSlider(audioOutput, this);
    volumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    //������ȡѡ�еĸ����к�
    connect(PlayList,SIGNAL(cellClicked(int,int)),this,SLOT(ChangRow(int)));
    //��˫���˲����б���ĸ����Ͳ��Ÿø���
    connect(PlayList, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableClicked(int)));
    //������������ý�����Ϣ��Ԫ��Ϣ������
    metaInformationResolver = new Phonon::MediaObject(this);
    //��Ҫ��AudioOutput�����Ӻ����ʹ��metaInformationResolver
    Phonon::AudioOutput *metaInformationAudioPutput =
            new Phonon::AudioOutput(Phonon::MusicCategory,this);
    Phonon::createPath(metaInformationResolver,metaInformationAudioPutput);
    //�����ı���״̬����metaStateChanged����
    connect(metaInformationResolver, SIGNAL(stateChanged(Phonon::State, Phonon::State)),
            this, SLOT(metaStateChanged(Phonon::State)));

    //��������Դ�����ı�ʱ����SourceChanged����
    connect(mediaObject,SIGNAL(currentSourceChanged(Phonon::MediaSource)),this,
            SLOT(SourceChanged(Phonon::MediaSource)));
    //������������󴫸�aboutTofinish����������һ�׸�
    connect(mediaObject,SIGNAL(aboutToFinish()),this,SLOT(aboutToFinish()));
    //��ȡ��һ�׸���������
    connect(mediaObject,SIGNAL(tick(qint64)),this,SLOT(GetNextMusicName()));
    //����ý������tick()�ź������²���ʱ�����ʾ
    mediaObject->setTickInterval(1000);//�����źŷ���ʱ����
    //ý�����mediaObject����ָ����ʱ��������tick�źţ��źŵ�ֵ�����ֵ�ʱ���ŵ�ʱ��
    connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(updateTime(qint64)));

    //��ȡ�������ŵ�״̬�����ı��������
    connect(mediaObject,SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this,SLOT(GetMusicName(Phonon::State)));

    //���ƽ��沼��
    MainLayout->addWidget(TitleLabel,0,0);
    MainLayout->addWidget(seekSlider,1,0,1,2);
    MainLayout->addLayout(LabelLayout,2,0,1,2);
    MainLayout->addLayout(ButtonLayout,3,0);
    MainLayout->addWidget(volumeSlider,3,1);
    MainLayout->setSizeConstraint(QLayout::SetFixedSize);//�û��޷��ı䴰�ڵĴ�С�����ִ�����Թ̶�
    //���岼��
    Layout = new QVBoxLayout(this);
    Layout->addLayout(MainLayout);
    Layout->addWidget(PlayList);
}

// ����ý��Դ�б����ݺ͵�ǰý��Դ��λ�����ı���������ư�ť��״̬
void Widget::changeActionState()
{
    // ���ý��Դ�б�Ϊ��
    if (sources.count() == 0)
    { 
        StartButton->setEnabled(false);
        StopButton->setEnabled(false);
        NextButton->setEnabled(false);
        LastButton->setEnabled(false);
    }
    else
    { // ���ý��Դ�б�Ϊ��
        StartButton->setEnabled(true);
        StopButton->setEnabled(true);
        // ���ý��Դ�б�ֻ��һ��
        if (sources.count() == 1)
        {
            NextButton->setEnabled(false);
            LastButton->setEnabled(false);
        }
        else
        { // ���ý��Դ�б��ж���
            NextButton->setEnabled(true);
            LastButton->setEnabled(true);
            int index = PlayList->currentRow();
            // ��������б�ǰѡ�е���Ϊ��һ��
            if (index == 0)
                LastButton->setEnabled(false);
            // ��������б�ǰѡ�е���Ϊ���һ��
            if (index + 1 == sources.count())
                NextButton->setEnabled(false);
        }
    }
}

void Widget::CreatePlayList()
{
     PlayList = new QTableWidget;
     // ����������Ŀ
     PlayList->setRowCount(0);
     PlayList->setColumnCount(3);
     // ���ñ�ͷ��ǩ
     QStringList list;
     list << tr("����") << tr("������") << tr("ʱ��");
     PlayList->setHorizontalHeaderLabels(list);
     //����ֻ��ѡ����
     PlayList->setSelectionMode(QAbstractItemView::SingleSelection);
     PlayList->setSelectionBehavior(QAbstractItemView::SelectRows);
     // ���ò���ʾ����
     PlayList->setShowGrid(false);
}

void Widget::CreateButton()
{
    ButtonLayout = new QHBoxLayout;

    QSize iconSize(40,40);//����ͼ���С
    StartButton = new QToolButton;
    StartButton->setIcon(QPixmap(":/image/3.png"));
    StartButton->setAutoRaise(true);//�����ڰ�ť�������ʾ��ť��Ӱ��
    StartButton->setIconSize(iconSize);
    connect(StartButton,SIGNAL(clicked()),this,SLOT(StartMusic()));

    StopButton = new QToolButton;
    StopButton->setIcon(QPixmap(":/image/2.png"));
    StopButton->setVisible(false);//���ذ�ť
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

    timeLabel= new QLabel;//��ʾ����ʱ��
    timeLabel->setText("00:00");
    TotaltimeLabel= new QLabel;//��ʾ����ʱ�䳤��
    TotaltimeLabel->setText("00:00");

    TitleLabel= new QLabel;//��ʾ��������
    TitleLabel->setText(tr("Music Title"));

    LabelLayout->addWidget(timeLabel);
    LabelLayout->addStretch();//�м�տ�
    LabelLayout->addWidget(TotaltimeLabel);
}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(tr("ɾ��"),this, SLOT(DelateMusic()));
    menu.addAction(tr("����б�"), this, SLOT(ClearPlayList()));
    menu.exec(event->globalPos());
}

Widget::~Widget()
{
    
}
