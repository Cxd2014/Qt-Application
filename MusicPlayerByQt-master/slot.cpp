#include "widget.h"

//各种槽函数

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
    Replay=QMessageBox::question(this,"Question",tr("你确定要全部清空列表吗?"),
                QMessageBox::Yes|QMessageBox::No);
    if(Replay==QMessageBox::Yes)
    {
        while (PlayList->rowCount())
            PlayList->removeRow(0);
        mediaObject->stop();//停止播放
        sources.clear();//清空播放源
        TitleLabel->setText(tr("Music Title"));
        StartButton->setEnabled(true);//使能
        StartButton->setVisible(true);
        StopButton->setEnabled(false);//失能
        StopButton->setVisible(false);//隐藏按钮
        changeActionState();
    }
}

void Widget::ChangRow(int R)
{
    row=R;
}

void Widget::DelateMusic()
{
    PlayList->removeRow(row);//删除列表中的歌曲
    sources.removeAt(row);//删除播放队列中的歌曲
}

void Widget::OpenFile()
{
    // 打开多个音乐文件
    QStringList List = QFileDialog::getOpenFileNames(this,tr("打开音乐文件"),
                                                     "d:\kugou");
    if(List.isEmpty())
        return;

    // 获取当前媒体源列表的大小
    int index = sources.size();
    // 将打开的音乐文件添加到媒体源列表后
    foreach (QString string, List)
    {
        Phonon::MediaSource source(string);
        sources.append(source);
    }
    // 这时会发射stateChanged()信号，从而调用metaStateChanged()函数进行媒体源的解析
    metaInformationResolver->setCurrentSource(sources.at(index));
}

// 当前媒体源播放将要结束时，如果在列表中当前媒体源的后面还有媒体源，
// 那么将它添加到播放队列中，否则停止播放
void Widget::aboutToFinish()
{
    int index = sources.indexOf(mediaObject->currentSource()) + 1;
    if (sources.size() > index)
    {
        mediaObject->enqueue(sources.at(index));
        // 跳转到歌曲最后
        mediaObject->seek(mediaObject->totalTime());
    }
    else
    {
        mediaObject->stop();
    }

}

// 当媒体源改变时，在播放列表中选中相应的行并更新图标的状态并获取歌曲名字
void Widget::SourceChanged(const Phonon::MediaSource &source)
{
    int index = sources.indexOf(source);
    PlayList->selectRow(index);
    changeActionState();
    //获取当前歌曲的名字
    TitleLabel->setText(QFileInfo(mediaObject->
                       currentSource().fileName()).baseName());
}

//解析媒体文件的元信息获取歌曲信息及将歌曲插入到播放列表
void Widget::metaStateChanged(Phonon::State newState)
{
    // 错误状态，则从媒体源列表中除去新添加的媒体源
    if(newState == Phonon::ErrorState)
    {
        QMessageBox::warning(this, tr("打开文件时出错"), metaInformationResolver->errorString());
        while (!sources.isEmpty() &&
               !(sources.takeLast() == metaInformationResolver->currentSource()))
        {};
        return;
    }

    // 如果既不处于停止状态也不处于暂停状态，则直接返回
    if(newState != Phonon::StoppedState && newState != Phonon::PausedState)
        return;

    // 如果媒体源类型错误，则直接返回
    if(metaInformationResolver->currentSource().type() == Phonon::MediaSource::Invalid)
        return;

    // 获取媒体信息
    QMap<QString, QString> metaData = metaInformationResolver->metaData();
    // 获取标题，如果为空，则使用文件名
    QString title = metaData.value("TITLE");
    if (title == "") {
        QString str = metaInformationResolver->currentSource().fileName();
        title = QFileInfo(str).baseName();
    }
    QTableWidgetItem *titleItem = new QTableWidgetItem(title);
    // 设置数据项不可编辑
    titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);
    // 获取艺术家信息
    QTableWidgetItem *artistItem = new QTableWidgetItem(metaData.value("ARTIST"));
    artistItem->setFlags(artistItem->flags() ^ Qt::ItemIsEditable);
    // 获取总时间信息
    qint64 totalTime = metaInformationResolver->totalTime();
    QTime time(0, (totalTime / 60000) % 60, (totalTime / 1000) % 60);
    QTableWidgetItem *timeItem = new QTableWidgetItem(time.toString("mm:ss"));

    // 插入到播放列表
    int currentRow = PlayList->rowCount();
    PlayList->insertRow(currentRow);
    PlayList->setItem(currentRow, 0, titleItem);
    PlayList->setItem(currentRow, 1, artistItem);
    PlayList->setItem(currentRow, 2, timeItem);

    // 如果添加的媒体源还没有解析完，那么继续解析下一个媒体源
    int index = sources.indexOf(metaInformationResolver->currentSource()) + 1;
    if (sources.size() > index)
    {
        metaInformationResolver->setCurrentSource(sources.at(index));
    }
    else
    { // 如果所有媒体源都已经解析完成
        // 如果播放列表中没有选中的行
        if (PlayList->selectedItems().isEmpty())
        {
            // 如果现在没有播放歌曲则设置第一个媒体源为媒体对象的当前媒体源
            //（因为可能正在播放歌曲时清空了播放列表，然后又添加了新的列表）
            if (mediaObject->state() != Phonon::PlayingState &&
                    mediaObject->state() != Phonon::PausedState)
            {
                mediaObject->setCurrentSource(sources.at(0));
            }
            else
            {
                //如果正在播放歌曲，则选中播放列表的第一个曲目,并更改图标状态
                PlayList->selectRow(0);
                changeActionState();
            }
        }
        else
        {
            // 如果播放列表中有选中的行，那么直接更新图标状态
            changeActionState();
        }
    }
}

// 双击播放列表
void Widget::tableClicked(int row)
{
    // 首先获取媒体对象当前的状态，然后停止播放并清空播放队列
    bool wasPlaying = mediaObject->state() == Phonon::PlayingState;
    mediaObject->stop();
    mediaObject->clearQueue();

    // 如果双击的播放列表中的行号大于媒体源列表的大小，则直接返回
    if(row >= sources.size())
        return;
    // 设置双击的行对应的媒体源为媒体对象的当前媒体源
    mediaObject->setCurrentSource(sources.at(row));
    // 如果以前媒体对象处于播放状态，那么开始播放选中的曲目
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
        case Phonon::ErrorState ://播放歌曲出现错去弹出警告对话框
            if(mediaObject->errorType() == Phonon::FatalError)
            {
                QMessageBox::warning(this, tr("致命错误"),
                                     mediaObject->errorString());
            }
            else
            {
                QMessageBox::warning(this, tr("错误"), mediaObject->errorString());
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
    qint64 Totaltime=mediaObject->totalTime();//获取歌曲总时间单位 ms
    //转换时间格式
    QTime Totaltime1(0, (Totaltime / 60000) % 60, (Totaltime / 1000) % 60);
    TotaltimeLabel->setText(Totaltime1.toString("mm:ss"));

    QTime Currenttime(0, (time / 60000) % 60, (time / 1000) % 60);
    timeLabel->setText(Currenttime.toString("mm:ss"));
}


void Widget::StopMusic()
{
    mediaObject->pause();
    StartButton->setEnabled(true);//使能
    StartButton->setVisible(true);
    StopButton->setEnabled(false);//失能
    StopButton->setVisible(false);//隐藏按钮
}


void Widget::StartMusic()
{
    mediaObject->play();
    StartButton->setEnabled(false);
    StartButton->setVisible(false);
    StopButton->setEnabled(true);
    StopButton->setVisible(true);
}
