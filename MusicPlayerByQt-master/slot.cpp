#include "widget.h"

//���ֲۺ���

void Widget::NextMusic()
{
    int index = sources.indexOf(mediaObject->currentSource());
    mediaObject->setCurrentSource(sources.at(index + 1));
    mediaObject->play();
}

void Widget::LastMusic()
{
    int index = sources.indexOf(mediaObject->currentSource());
    mediaObject->setCurrentSource(sources.at(index - 1));
    mediaObject->play();
}

void Widget::ClearPlayList()
{
    QMessageBox::StandardButton Replay;
    Replay=QMessageBox::question(this,"Question",tr("��ȷ��Ҫȫ������б���?"),
                QMessageBox::Yes|QMessageBox::No);
    if(Replay==QMessageBox::Yes)
    {
        while (PlayList->rowCount())
            PlayList->removeRow(0);
        mediaObject->stop();//ֹͣ����
        sources.clear();//��ղ���Դ
        TitleLabel->setText(tr("Music Title"));
        StartButton->setEnabled(true);//ʹ��
        StartButton->setVisible(true);
        StopButton->setEnabled(false);//ʧ��
        StopButton->setVisible(false);//���ذ�ť
        changeActionState();
    }
}

void Widget::ChangRow(int R)
{
    row=R;
}

void Widget::DelateMusic()
{
    PlayList->removeRow(row);//ɾ���б��еĸ���
    sources.removeAt(row);//ɾ�����Ŷ����еĸ���
}

void Widget::OpenFile()
{
    // �򿪶�������ļ�
    QStringList List = QFileDialog::getOpenFileNames(this,tr("�������ļ�"),
                                                     "d:\kugou");
    if(List.isEmpty())
        return;

    // ��ȡ��ǰý��Դ�б�Ĵ�С
    int index = sources.size();
    // ���򿪵������ļ���ӵ�ý��Դ�б��
    foreach (QString string, List)
    {
        Phonon::MediaSource source(string);
        sources.append(source);
    }
    // ��ʱ�ᷢ��stateChanged()�źţ��Ӷ�����metaStateChanged()��������ý��Դ�Ľ���
    metaInformationResolver->setCurrentSource(sources.at(index));
}

// ��ǰý��Դ���Ž�Ҫ����ʱ��������б��е�ǰý��Դ�ĺ��滹��ý��Դ��
// ��ô������ӵ����Ŷ����У�����ֹͣ����
void Widget::aboutToFinish()
{
    int index = sources.indexOf(mediaObject->currentSource()) + 1;
    if (sources.size() > index)
    {
        mediaObject->enqueue(sources.at(index));
        // ��ת���������
        mediaObject->seek(mediaObject->totalTime());
    }
    else
    {
        mediaObject->stop();
    }

}

// ��ý��Դ�ı�ʱ���ڲ����б���ѡ����Ӧ���в�����ͼ���״̬����ȡ��������
void Widget::SourceChanged(const Phonon::MediaSource &source)
{
    int index = sources.indexOf(source);
    PlayList->selectRow(index);
    changeActionState();
    //��ȡ��ǰ����������
    TitleLabel->setText(QFileInfo(mediaObject->
                       currentSource().fileName()).baseName());
}

//����ý���ļ���Ԫ��Ϣ��ȡ������Ϣ�����������뵽�����б�
void Widget::metaStateChanged(Phonon::State newState)
{
    // ����״̬�����ý��Դ�б��г�ȥ����ӵ�ý��Դ
    if(newState == Phonon::ErrorState)
    {
        QMessageBox::warning(this, tr("���ļ�ʱ����"), metaInformationResolver->errorString());
        while (!sources.isEmpty() &&
               !(sources.takeLast() == metaInformationResolver->currentSource()))
        {};
        return;
    }

    // ����Ȳ�����ֹͣ״̬Ҳ��������ͣ״̬����ֱ�ӷ���
    if(newState != Phonon::StoppedState && newState != Phonon::PausedState)
        return;

    // ���ý��Դ���ʹ�����ֱ�ӷ���
    if(metaInformationResolver->currentSource().type() == Phonon::MediaSource::Invalid)
        return;

    // ��ȡý����Ϣ
    QMap<QString, QString> metaData = metaInformationResolver->metaData();
    // ��ȡ���⣬���Ϊ�գ���ʹ���ļ���
    QString title = metaData.value("TITLE");
    if (title == "") {
        QString str = metaInformationResolver->currentSource().fileName();
        title = QFileInfo(str).baseName();
    }
    QTableWidgetItem *titleItem = new QTableWidgetItem(title);
    // ����������ɱ༭
    titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);
    // ��ȡ��������Ϣ
    QTableWidgetItem *artistItem = new QTableWidgetItem(metaData.value("ARTIST"));
    artistItem->setFlags(artistItem->flags() ^ Qt::ItemIsEditable);
    // ��ȡ��ʱ����Ϣ
    qint64 totalTime = metaInformationResolver->totalTime();
    QTime time(0, (totalTime / 60000) % 60, (totalTime / 1000) % 60);
    QTableWidgetItem *timeItem = new QTableWidgetItem(time.toString("mm:ss"));

    // ���뵽�����б�
    int currentRow = PlayList->rowCount();
    PlayList->insertRow(currentRow);
    PlayList->setItem(currentRow, 0, titleItem);
    PlayList->setItem(currentRow, 1, artistItem);
    PlayList->setItem(currentRow, 2, timeItem);

    // �����ӵ�ý��Դ��û�н����꣬��ô����������һ��ý��Դ
    int index = sources.indexOf(metaInformationResolver->currentSource()) + 1;
    if (sources.size() > index)
    {
        metaInformationResolver->setCurrentSource(sources.at(index));
    }
    else
    { // �������ý��Դ���Ѿ��������
        // ��������б���û��ѡ�е���
        if (PlayList->selectedItems().isEmpty())
        {
            // �������û�в��Ÿ��������õ�һ��ý��ԴΪý�����ĵ�ǰý��Դ
            //����Ϊ�������ڲ��Ÿ���ʱ����˲����б�Ȼ����������µ��б�
            if (mediaObject->state() != Phonon::PlayingState &&
                    mediaObject->state() != Phonon::PausedState)
            {
                mediaObject->setCurrentSource(sources.at(0));
            }
            else
            {
                //������ڲ��Ÿ�������ѡ�в����б�ĵ�һ����Ŀ,������ͼ��״̬
                PlayList->selectRow(0);
                changeActionState();
            }
        }
        else
        {
            // ��������б�����ѡ�е��У���ôֱ�Ӹ���ͼ��״̬
            changeActionState();
        }
    }
}

// ˫�������б�
void Widget::tableClicked(int row)
{
    // ���Ȼ�ȡý�����ǰ��״̬��Ȼ��ֹͣ���Ų���ղ��Ŷ���
    bool wasPlaying = mediaObject->state() == Phonon::PlayingState;
    mediaObject->stop();
    mediaObject->clearQueue();

    // ���˫���Ĳ����б��е��кŴ���ý��Դ�б�Ĵ�С����ֱ�ӷ���
    if(row >= sources.size())
        return;
    // ����˫�����ж�Ӧ��ý��ԴΪý�����ĵ�ǰý��Դ
    mediaObject->setCurrentSource(sources.at(row));
    // �����ǰý������ڲ���״̬����ô��ʼ����ѡ�е���Ŀ
    if (wasPlaying)
    {
        mediaObject->play();
    }
    else
    {
        mediaObject->play();
        StartButton->setEnabled(false);
        StartButton->setVisible(false);
        StopButton->setEnabled(true);
        StopButton->setVisible(true);
    }

}

void Widget::GetNextMusicname()
{
    TitleLabel->setText(QFileInfo(mediaObject->
                       currentSource().fileName()).baseName());
}

void Widget::GetMusicName(Phonon::State newState)
{

    switch (newState)
    {
        case Phonon::ErrorState ://���Ÿ������ִ�ȥ��������Ի���
            if(mediaObject->errorType() == Phonon::FatalError)
            {
                QMessageBox::warning(this, tr("��������"),
                                     mediaObject->errorString());
            }
            else
            {
                QMessageBox::warning(this, tr("����"), mediaObject->errorString());
            }
            break;

        case Phonon::PlayingState :
             TitleLabel->setText(QFileInfo(mediaObject->
                                currentSource().fileName()).baseName());
            break;

        case Phonon::PausedState :
             TitleLabel->setText(QFileInfo(mediaObject->
                                 currentSource().fileName()).baseName() + tr("--Pause"));
            break;

        default :
            break;
    }
}

void Widget::updateTime(qint64 time)
{
    qint64 Totaltime=mediaObject->totalTime();//��ȡ������ʱ�䵥λ ms
    //ת��ʱ���ʽ
    QTime Totaltime1(0, (Totaltime / 60000) % 60, (Totaltime / 1000) % 60);
    TotaltimeLabel->setText(Totaltime1.toString("mm:ss"));

    QTime Currenttime(0, (time / 60000) % 60, (time / 1000) % 60);
    timeLabel->setText(Currenttime.toString("mm:ss"));
}


void Widget::StopMusic()
{
    mediaObject->pause();
    StartButton->setEnabled(true);//ʹ��
    StartButton->setVisible(true);
    StopButton->setEnabled(false);//ʧ��
    StopButton->setVisible(false);//���ذ�ť
}


void Widget::StartMusic()
{
    mediaObject->play();
    StartButton->setEnabled(false);
    StartButton->setVisible(false);
    StopButton->setEnabled(true);
    StopButton->setVisible(true);
}
