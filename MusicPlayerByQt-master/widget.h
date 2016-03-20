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

    int row;//��Ų����б���ѡ�е���
    //QString MusicFilePath;//���������ļ�·��
private:
    //���¶�������һ�ʱ��,ʹ�����Ҽ����ֲ˵���
    void contextMenuEvent(QContextMenuEvent *event);

private slots:

    void ChangRow(int R);//�ı�ѡ�е�����
    void StopMusic();//ֹͣ���ֲ���
    void StartMusic();//��ʼ���ֲ���
    void NextMusic();//��һ�׸���
    void LastMusic();//��һ�׸���
    void OpenFile();//�������ļ�
    void updateTime(qint64 time);//������ʾ����ʱ��
    void GetMusicName(Phonon::State newState);//��ȡ���������ֲ��жϲ���״̬�Ƿ���ȷ
    void GetNextMusicname();//��ȡ��һ�׸�ĸ���
    void ClearPlayList();//��ղ����б�
    void DelateMusic();//�Ӳ����б���ɾ����ѡ�и���

    //��ý��Դ�ı�ʱ���ڲ����б���ѡ����Ӧ���в�����ͼ���״̬����ȡ��������
    void SourceChanged(const Phonon::MediaSource &source);
    void aboutToFinish();//��һ�׸��������ָ��������һ�׸�

    //����ý���ļ���Ԫ��Ϣ��ȡ������Ϣ�����������뵽�����б�
    void metaStateChanged(Phonon::State newState);
    void tableClicked(int row);//��˫���˲����б���ĸ����Ͳ��Ÿø���

private:
    void CreateButton();//������ť
    void CreateMusicPlayer();//�������ֲ�����
    void CreatePlayList();//�������Ÿ����б�
    void changeActionState();//����ý��Դ�б����ݺ͵�ǰý��Դ��λ�����ı���������ư�ť��״̬

    Phonon::MediaObject *mediaObject;//��������������
    Phonon::SeekSlider *seekSlider;//�������Ʋ��Ž��ȵĻ���
    Phonon::VolumeSlider *volumeSlider;//�������Ʋ���

    QTableWidget *PlayList;//���������б�
    QList<Phonon::MediaSource> sources;//����һ������Դ
    //������������ý�����Ϣ��Ԫ��Ϣ������
    Phonon::MediaObject *metaInformationResolver;

    QLabel *TitleLabel;//��ʾ��������
    QLabel *timeLabel;//��ʾ����ʱ��
    QLabel *TotaltimeLabel;//��ʾ����ʱ�䳤��

    QToolButton *StartButton;
    QToolButton *StopButton;
    QToolButton *NextButton;
    QToolButton *LastButton;
    QToolButton *OpenButton;

    QGridLayout *MainLayout;//���ֹ���
    QVBoxLayout *Layout;
    QHBoxLayout *ButtonLayout;
    QHBoxLayout *LabelLayout;

};

#endif // WIDGET_H
