#include "todolist.h"
#include "ui_todolist.h"
#include <QThread>


todoList::todoList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::todoList)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    QDate curDate = QDate::currentDate();
    ui->dateEdit->setDate(curDate);
    QTime curTime = QTime::currentTime();
    ui->timeEdit->setTime(curTime);

    initLayout();

    // 创建并启动定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &todoList::checkDateTime);
    timer->start(1000);
}

todoList::~todoList()
{
    delete ui;
}

void todoList::initLayout()
{
    scrollAreaAll = new QScrollArea();
    centralWidgetAll = new QWidget(scrollAreaAll);
    flowLayout = new FlowLayout(centralWidgetAll);
    vlayoutAll = new QVBoxLayout(ui->page_all);

    scrollAreaCur = new QScrollArea();
    centralWidgetCur = new QWidget(scrollAreaCur);
    flowLayoutCur = new FlowLayout(centralWidgetCur);
    vlayoutCur = new QVBoxLayout(ui->page_today);

    scrollAreaFt = new QScrollArea();
    centralWidgetFt = new QWidget(scrollAreaFt);
    flowLayoutFt = new FlowLayout(centralWidgetFt);
    vlayoutFt = new QVBoxLayout(ui->page_future);

    scrollAreaZy = new QScrollArea();
    centralWidgetZy = new QWidget(scrollAreaZy);
    flowLayoutZy = new FlowLayout(centralWidgetZy);
    vlayoutZy = new QVBoxLayout(ui->page_important);

    scrollAreaFs = new QScrollArea();
    centralWidgetFs = new QWidget(scrollAreaFs);
    flowLayoutFs = new FlowLayout(centralWidgetFs);
    vlayoutFs = new QVBoxLayout(ui->page_finish);
}

void todoList::checkDateTime()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();

    QString rootDir = QCoreApplication::applicationDirPath();
    QString xmlFilePath = QDir(rootDir).filePath("config/todo/todo.xml");
    updatePage_curWidget(xmlFilePath);

    if(currentDateTime.toSecsSinceEpoch() == targetDatetime.toSecsSinceEpoch())
    {
        qDebug() << currentDateTime << "--" << targetDatetime;
        timer->stop(); // 停止计时器
        QMetaObject::invokeMethod(this, "showReminder", Qt::QueuedConnection);
    }

}

void todoList::showReminder()
{
    remind = new todoRemind(this);
    remind->setModal(true);
    int result = remind->exec();
    delete remind;
    remind = nullptr;
    // todoRemind remind;

    if(result == QDialog::Accepted)
    {
        // 更新目标时间，假设为当前时间加10秒
        targetDatetime = QDateTime::currentDateTime().addSecs(10);

        // 重新启动定时器
        timer->start(1000);
    }

}

void todoList::on_pb_firstDay_clicked()
{
    QDate curDate = QDate::currentDate();
    ui->dateEdit->setDate(curDate);
}

void todoList::on_pb_nextDay_clicked()
{
    QDate curDate = QDate::currentDate();
    QDate nextDay = curDate.addDays(1);
    ui->dateEdit->setDate(nextDay);
}

void todoList::on_pb_add_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void todoList::on_pb_today_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    QString rootDir = QCoreApplication::applicationDirPath();
    QString xmlFilePath = QDir(rootDir).filePath("config/todo/todo.xml");
    updatePage_curWidget(xmlFilePath);
}

void todoList::on_pb_future_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    QString rootDir = QCoreApplication::applicationDirPath();
    QString xmlFilePath = QDir(rootDir).filePath("config/todo/todo.xml");

    updatePage_futureWidget(xmlFilePath);
}

void todoList::on_pb_important_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

    QString rootDir = QCoreApplication::applicationDirPath();
    QString xmlFilePath = QDir(rootDir).filePath("config/todo/todo.xml");

    updatePage_zyWidget(xmlFilePath);
}

void todoList::on_pb_all_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    // clearPage_allWidget();

    QString rootDir = QCoreApplication::applicationDirPath();
    QString xmlFilePath = QDir(rootDir).filePath("config/todo/todo.xml");

    updatePage_allWidget(xmlFilePath);
}

void todoList::on_pb_finish_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);

    QString rootDir = QCoreApplication::applicationDirPath();
    QString xmlFilePath = QDir(rootDir).filePath("config/todo/todo.xml");

    updatePage_fsWidget(xmlFilePath);
}

void todoList::on_pb_sure_clicked()
{
    QString rootDir = QCoreApplication::applicationDirPath();
    QString xmlFilePath = QDir(rootDir).filePath("config/todo/todo.xml");
    QFileInfo file(xmlFilePath);
    if(!file.exists())
    {
        createXmlFile(xmlFilePath);
        appendXmlInfo(xmlFilePath);
    }
    else
    {
        appendXmlInfo(xmlFilePath);
    }
}

void todoList::createXmlFile(const QString &file)
{
    QDomDocument document;

    // 创建 XML 声明
    QDomProcessingInstruction xmlDeclaration = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    document.appendChild(xmlDeclaration);

    // 创建根元素
    QDomElement root = document.createElement("root");
    document.appendChild(root);

    // 打开文件并写入 XML 内容
    QFile xmlFile(file);
    if (!xmlFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // qDebug() << "Failed to open file for writing:" << file;
        return;
    }

    // 使用 QTextStream 将 QDomDocument 写入文件
    QTextStream stream(&xmlFile);
    stream << document.toString();
    xmlFile.close();

    // qDebug() << "XML file created successfully:" << file;
}

void todoList::appendXmlInfo(const QString &file)
{
    QFile xmlFile(file);
    if (!xmlFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Failed to open file for reading/writing:" << file;
        return;
    }

    // 读取现有的 XML 文档
    QDomDocument document;
    if (!document.setContent(&xmlFile))
    {
        qDebug() << "Failed to load XML content from file:" << file;
        xmlFile.close();
        return;
    }

    // 获取根元素
    QDomElement root = document.documentElement();

    // 创建新的任务元素
    QDomElement todoElement = document.createElement("todo");
    // 添加 name 元素
    int id = getXmlNextId(file);
    QDomElement idElement = document.createElement("id");
    QDomText idText = document.createTextNode(QString::number(id));
    idElement.appendChild(idText);
    todoElement.appendChild(idElement);

    // 添加 date 元素
    QDomElement dateElement = document.createElement("date");
    QString date = ui->dateEdit->date().toString("yyyy-MM-dd");
    QDomText dateText = document.createTextNode(date);
    dateElement.appendChild(dateText);
    todoElement.appendChild(dateElement);

    QDomElement timeElement = document.createElement("time");
    QString time = ui->timeEdit->time().toString("HH:mm");
    QDomText timeText = document.createTextNode(time);
    timeElement.appendChild(timeText);
    todoElement.appendChild(timeElement);

    QDomElement titleElement = document.createElement("title");
    QString title = ui->l_title->text();
    QDomText titleText = document.createTextNode(title);
    titleElement.appendChild(titleText);
    todoElement.appendChild(titleElement);

    QDomElement descElement = document.createElement("notes");
    QString desc = ui->pte_notes->toPlainText();
    QDomText descText = document.createTextNode(desc);
    descElement.appendChild(descText);
    todoElement.appendChild(descElement);

    QDomElement zyElement = document.createElement("flag");
    QString zy;
    if(ui->ck_important->isChecked())
        zy = "1";
    else
        zy = "0";
    QDomText zyText = document.createTextNode(zy);
    zyElement.appendChild(zyText);
    todoElement.appendChild(zyElement);

    // 将任务元素添加到根元素
    root.appendChild(todoElement);

    if(ui->l_title->text().isEmpty())
    {
        QMessageBox::warning(NULL, tr("警告"), tr("标题内容不允许为空！"));
    }
    else
    {
        // 移动文件指针到开始位置并清空文件内容
        xmlFile.resize(0);
        // 写入修改后的 XML 内容
        QTextStream stream(&xmlFile);
        stream << document.toString();
        xmlFile.close();
    }
}

int todoList::getXmlNextId(const QString &file)
{
    QFile xmlFile(file);
    if(!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for reading:" << file;
        return 1; // 如果无法读取文件，则从 1 开始
    }

    QDomDocument document;
    if(!document.setContent(&xmlFile))
    {
        qDebug() << "Failed to load XML content from file:" << file;
        xmlFile.close();
        return 1; // 如果文件没有内容，则从 1 开始
    }
    xmlFile.close();

    QDomElement root = document.documentElement();
    if(root.isNull() || root.tagName() != "root")
    {
        qDebug() << "Invalid XML format or missing root element 'ROOT'";
        return 1; // 如果 XML 结构不正确，则从 1 开始
    }

    // 遍历所有的 TODO 元素，找到最大的 id
    int maxId = 0;
    QDomNodeList todos = root.elementsByTagName("todo");
    for(int i = 0; i < todos.count(); ++i)
    {
        QDomElement todo = todos.at(i).toElement();
        QDomElement idElement = todo.firstChildElement("id");
        if(!idElement.isNull())
        {
            int id = idElement.text().toInt();
            if(id > maxId)
            {
                maxId = id;
            }
        }
    }

    // 返回最大 id 加 1 作为下一个 id
    return maxId + 1;
}

int todoList::getTodoCountFromFile(const QString &file)
{
    QFile xmlFile(file);
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << file;
        return 0;
    }

    QDomDocument document;
    if (!document.setContent(&xmlFile)) {
        qDebug() << "Failed to load XML content from file:" << file;
        xmlFile.close();
        return 0;
    }
    xmlFile.close();

    QDomElement root = document.documentElement();
    if (root.isNull() || root.tagName() != "root") {
        qDebug() << "Invalid XML format or missing root element 'ROOT'";
        return 0;
    }

    // 获取所有的 TODO 元素
    QDomNodeList todos = root.elementsByTagName("todo");
    return todos.count(); // 返回 TODO 元素的数量
}

void todoList::updatePage_curWidget(const QString &file)
{
    clearPage_curWidget();
    loadTodoXmlFileCurInfo(file);
}

void todoList::updatePage_allWidget(const QString &file)
{
    clearPage_allWidget();
    loadTodoXmlFileAllInfo(file);
}

void todoList::updatePage_futureWidget(const QString &file)
{
    clearPage_ftWidget();
    loadTodoXmlFileFtInfo(file);
}

void todoList::updatePage_zyWidget(const QString &file)
{
    clearPage_zyWidget();
    loadTodoXmlFileZyInfo(file);
}

void todoList::updatePage_fsWidget(const QString &file)
{
    clearPage_fsWidget();
    loadTodoXmlFileFsInfo(file);
}

void todoList::clearPage_curWidget()
{
    QLayoutItem *item;
    while((item = flowLayoutCur->takeAt(0)) != nullptr)
    {
        delete item->widget(); // 删除小部件
        delete item;           // 删除布局项
    }
}

void todoList::clearPage_allWidget()
{
    QLayoutItem *item;
    while((item = flowLayout->takeAt(0)) != nullptr)
    {
        delete item->widget(); // 删除小部件
        delete item;           // 删除布局项
    }
}

void todoList::clearPage_ftWidget()
{
    QLayoutItem *item;
    while((item = flowLayoutFt->takeAt(0)) != nullptr)
    {
        delete item->widget(); // 删除小部件
        delete item;           // 删除布局项
    }
}

void todoList::clearPage_zyWidget()
{
    QLayoutItem *item;
    while((item = flowLayoutZy->takeAt(0)) != nullptr)
    {
        delete item->widget(); // 删除小部件
        delete item;           // 删除布局项
    }
}

void todoList::clearPage_fsWidget()
{
    QLayoutItem *item;
    while((item = flowLayoutFs->takeAt(0)) != nullptr)
    {
        delete item->widget(); // 删除小部件
        delete item;           // 删除布局项
    }
}

void todoList::loadTodoXmlFileCurInfo(const QString &file)
{
    QFile xmlFile(file);
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << file;
    }

    QDomDocument document;
    if (!document.setContent(&xmlFile)) {
        qDebug() << "Failed to load XML content from file:" << file;
        xmlFile.close();
    }
    xmlFile.close();

    QDomElement root = document.documentElement();
    if (root.isNull() || root.tagName() != "root") {
        qDebug() << "Invalid XML format or missing root element 'ROOT'";
    }

    QDomNodeList todos = root.elementsByTagName("todo");
    for (int i = 0; i < todos.count(); ++i)
    {
        QDomElement todoElement = todos.at(i).toElement();
        getTodoXmlFileCurInfo(todoElement); // 显示 TODO 的详细信息
    }
}

void todoList::loadTodoXmlFileAllInfo(const QString &file)
{
    QFile xmlFile(file);
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << file;
    }

    QDomDocument document;
    if (!document.setContent(&xmlFile)) {
        qDebug() << "Failed to load XML content from file:" << file;
        xmlFile.close();
    }
    xmlFile.close();

    QDomElement root = document.documentElement();
    if (root.isNull() || root.tagName() != "root") {
        qDebug() << "Invalid XML format or missing root element 'ROOT'";
    }

    QDomNodeList todos = root.elementsByTagName("todo");
    for (int i = 0; i < todos.count(); ++i)
    {
        QDomElement todoElement = todos.at(i).toElement();
        getTodoXmlFileAllInfo(todoElement); // 显示 TODO 的详细信息
    }
}

void todoList::loadTodoXmlFileFtInfo(const QString &file)
{
    QFile xmlFile(file);
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << file;
    }

    QDomDocument document;
    if (!document.setContent(&xmlFile)) {
        qDebug() << "Failed to load XML content from file:" << file;
        xmlFile.close();
    }
    xmlFile.close();

    QDomElement root = document.documentElement();
    if (root.isNull() || root.tagName() != "root") {
        qDebug() << "Invalid XML format or missing root element 'ROOT'";
    }

    QDomNodeList todos = root.elementsByTagName("todo");
    for (int i = 0; i < todos.count(); ++i)
    {
        QDomElement todoElement = todos.at(i).toElement();
        getTodoXmlFileFtInfo(todoElement); // 显示 TODO 的详细信息
    }
}

void todoList::loadTodoXmlFileZyInfo(const QString &file)
{
    QFile xmlFile(file);
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << file;
    }

    QDomDocument document;
    if (!document.setContent(&xmlFile)) {
        qDebug() << "Failed to load XML content from file:" << file;
        xmlFile.close();
    }
    xmlFile.close();

    QDomElement root = document.documentElement();
    if (root.isNull() || root.tagName() != "root") {
        qDebug() << "Invalid XML format or missing root element 'ROOT'";
    }

    QDomNodeList todos = root.elementsByTagName("todo");
    for (int i = 0; i < todos.count(); ++i)
    {
        QDomElement todoElement = todos.at(i).toElement();
        getTodoXmlFileZyInfo(todoElement); // 显示 TODO 的详细信息
    }
}

void todoList::loadTodoXmlFileFsInfo(const QString &file)
{
    QFile xmlFile(file);
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << file;
    }

    QDomDocument document;
    if (!document.setContent(&xmlFile)) {
        qDebug() << "Failed to load XML content from file:" << file;
        xmlFile.close();
    }
    xmlFile.close();

    QDomElement root = document.documentElement();
    if (root.isNull() || root.tagName() != "root") {
        qDebug() << "Invalid XML format or missing root element 'ROOT'";
    }

    QDomNodeList todos = root.elementsByTagName("todo");
    for (int i = 0; i < todos.count(); ++i)
    {
        QDomElement todoElement = todos.at(i).toElement();
        getTodoXmlFileFsInfo(todoElement); // 显示 TODO 的详细信息
    }
}

void todoList::getTodoXmlFileCurInfo(const QDomElement &todoElement)
{
    QString date = todoElement.firstChildElement("date").text();
    QDate date_D = QDate::fromString(date, "yyyy-MM-dd");
    QString time = todoElement.firstChildElement("time").text();
    QTime time_T = QTime::fromString(time, "HH:mm");

    targetDatetime = QDateTime(date_D , time_T);

    if(date_D == QDate::currentDate())
    {
        QString id = todoElement.firstChildElement("id").text();
        // QString time = todoElement.firstChildElement("time").text();
        QString title = todoElement.firstChildElement("title").text();
        QString notes = todoElement.firstChildElement("notes").text();
        QString flag = todoElement.firstChildElement("flag").text();
        layout_curInfo = new QVBoxLayout(ui->page_today);

        QLabel *l_date = new QLabel(QString("id:%1\nDateTime: %2 %3").arg(id, date, time));
        QLabel *l_notes = new QLabel(QString("title: %1\nnotes: %2").arg(title, notes));

        layout_curInfo->addWidget(l_date);
        layout_curInfo->addWidget(l_notes);
        if(flag.toInt() == 1)
        {

            QLabel *l_zy = new QLabel;
            QPixmap pixmap(":/tool/tool/transition.png");  // 使用资源文件路径
            l_zy->setPixmap(pixmap);
            l_zy->setScaledContents(true);
            l_zy->setFixedSize(pixmap.size());

            layout_curInfo->addWidget(l_zy);
        }
        l_date->setStyleSheet("QLabel {"
                              "color: #333333;" /* 文本颜色 */
                              "font-size: 14px;" /* 字体大小 */
                              "font-weight: bold;" /* 字体粗细 */
                              "font-family: Arial;" /* 字体家族 */
                              "background-color: #f0f0f0;" /* 背景颜色 */
                              "border: 2px solid #1c7cd6;" /* 边框颜色 */
                              "border-radius: 5px;" /* 边框圆角 */
                              "padding: 10px;" /* 内边距 */
                              "}");

        l_notes->setStyleSheet("QLabel {"
                               "color: #333333;" /* 文本颜色 */
                               "font-size: 14px;" /* 字体大小 */
                               "font-weight: bold;" /* 字体粗细 */
                               "font-family: Arial;" /* 字体家族 */
                               "background-color: #f0f0f0;" /* 背景颜色 */
                               "border: 2px solid #1c7cd6;" /* 边框颜色 */
                               "border-radius: 5px;" /* 边框圆角 */
                               "padding: 10px;" /* 内边距 */
                               "}");

        QWidget *widget = new QWidget;
        widget->setLayout(layout_curInfo);
        widget->setStyleSheet("QWidget {"
                              "border: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #1c7cd6, stop:1 #ff5722);" /* 渐变边框 */
                              "border-radius: 5px;" /* 边框圆角 */
                              "background-color: #f0f0f0;" /* 背景颜色 */
                              "}");
        flowLayoutCur->addWidget(widget);

        centralWidgetCur->setLayout(flowLayoutCur);
        centralWidgetCur->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);  // 确保流布局内部控件的大小
        scrollAreaCur->setWidget(centralWidgetCur);
        scrollAreaCur->setWidgetResizable(true);  // 允许内容自动调整大小
        vlayoutCur->addWidget(scrollAreaCur);
        setLayout(vlayoutCur);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
}

void todoList::getTodoXmlFileAllInfo(const QDomElement &todoElement)
{
    layout_allInfo = new QVBoxLayout;
    QString id = todoElement.firstChildElement("id").text();
    QString date = todoElement.firstChildElement("date").text();
    QString time = todoElement.firstChildElement("time").text();
    QString title = todoElement.firstChildElement("title").text();
    QString notes = todoElement.firstChildElement("notes").text();
    QString flag = todoElement.firstChildElement("flag").text();
    QLabel *l_date = new QLabel(QString("id:%1\nDateTime: %2 %3").arg(id, date, time));
    QLabel *l_notes = new QLabel(QString("title: %1\nnotes: %2").arg(title, notes));

    layout_allInfo->addWidget(l_date);
    layout_allInfo->addWidget(l_notes);
    if(flag.toInt() == 1)
    {

        QLabel *l_zy = new QLabel;
        QPixmap pixmap(":/tool/tool/transition.png");  // 使用资源文件路径
        l_zy->setPixmap(pixmap);
        l_zy->setScaledContents(true);
        l_zy->setFixedSize(pixmap.size());

        layout_allInfo->addWidget(l_zy);
    }
    l_date->setStyleSheet("QLabel {"
                          "color: #333333;" /* 文本颜色 */
                          "font-size: 14px;" /* 字体大小 */
                          "font-weight: bold;" /* 字体粗细 */
                          "font-family: Arial;" /* 字体家族 */
                          "background-color: #f0f0f0;" /* 背景颜色 */
                          "border: 2px solid #1c7cd6;" /* 边框颜色 */
                          "border-radius: 5px;" /* 边框圆角 */
                          "padding: 10px;" /* 内边距 */
                          "}");
    l_notes->setStyleSheet("QLabel {"
                          "color: #333333;" /* 文本颜色 */
                          "font-size: 14px;" /* 字体大小 */
                          "font-weight: bold;" /* 字体粗细 */
                          "font-family: Arial;" /* 字体家族 */
                          "background-color: #f0f0f0;" /* 背景颜色 */
                          "border: 2px solid #1c7cd6;" /* 边框颜色 */
                          "border-radius: 5px;" /* 边框圆角 */
                          "padding: 10px;" /* 内边距 */
                          "}");

    QWidget *widget = new QWidget;
    widget->setStyleSheet("QWidget {"
                        "border: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #1c7cd6, stop:1 #ff5722);" /* 渐变边框 */
                        "border-radius: 5px;" /* 边框圆角 */
                        "background-color: #f0f0f0;" /* 背景颜色 */
                        "}");
    widget->setLayout(layout_allInfo);
    flowLayout->addWidget(widget);

    centralWidgetAll->setLayout(flowLayout);
    centralWidgetAll->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);  // 确保流布局内部控件的大小
    scrollAreaAll->setWidget(centralWidgetAll);
    scrollAreaAll->setWidgetResizable(true);  // 允许内容自动调整大小
    vlayoutAll->addWidget(scrollAreaAll);
    setLayout(vlayoutAll);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void todoList::getTodoXmlFileFtInfo(const QDomElement &todoElement)
{
    QString date = todoElement.firstChildElement("date").text();
    QDate date_D = QDate::fromString(date, "yyyy-MM-dd");
    if(date_D > QDate::currentDate())
    {
        QString id = todoElement.firstChildElement("id").text();
        QString time = todoElement.firstChildElement("time").text();
        QString title = todoElement.firstChildElement("title").text();
        QString notes = todoElement.firstChildElement("notes").text();
        QString flag = todoElement.firstChildElement("flag").text();
        layout_ftInfo = new QVBoxLayout;

        QLabel *l_date = new QLabel(QString("id:%1\nDateTime: %2 %3").arg(id, date, time));
        QLabel *l_notes = new QLabel(QString("title: %1\nnotes: %2").arg(title, notes));

        layout_ftInfo->addWidget(l_date);
        layout_ftInfo->addWidget(l_notes);
        if(flag.toInt() == 1)
        {

            QLabel *l_zy = new QLabel;
            QPixmap pixmap(":/tool/tool/transition.png");  // 使用资源文件路径
            l_zy->setPixmap(pixmap);
            l_zy->setScaledContents(true);
            l_zy->setFixedSize(pixmap.size());

            layout_ftInfo->addWidget(l_zy);
        }
        l_date->setStyleSheet("QLabel {"
                              "color: #333333;" /* 文本颜色 */
                              "font-size: 14px;" /* 字体大小 */
                              "font-weight: bold;" /* 字体粗细 */
                              "font-family: Arial;" /* 字体家族 */
                              "background-color: #f0f0f0;" /* 背景颜色 */
                              "border: 2px solid #1c7cd6;" /* 边框颜色 */
                              "border-radius: 5px;" /* 边框圆角 */
                              "padding: 10px;" /* 内边距 */
                              "}");

        l_notes->setStyleSheet("QLabel {"
                               "color: #333333;" /* 文本颜色 */
                               "font-size: 14px;" /* 字体大小 */
                               "font-weight: bold;" /* 字体粗细 */
                               "font-family: Arial;" /* 字体家族 */
                               "background-color: #f0f0f0;" /* 背景颜色 */
                               "border: 2px solid #1c7cd6;" /* 边框颜色 */
                               "border-radius: 5px;" /* 边框圆角 */
                               "padding: 10px;" /* 内边距 */
                               "}");

        QWidget *widget = new QWidget;
        widget->setLayout(layout_ftInfo);
        widget->setStyleSheet("QWidget {"
                              "border: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #1c7cd6, stop:1 #ff5722);" /* 渐变边框 */
                              "border-radius: 5px;" /* 边框圆角 */
                              "background-color: #f0f0f0;" /* 背景颜色 */
                              "}");
        flowLayoutFt->addWidget(widget);

        centralWidgetFt->setLayout(flowLayoutFt);
        centralWidgetFt->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);  // 确保流布局内部控件的大小
        scrollAreaFt->setWidget(centralWidgetFt);
        scrollAreaFt->setWidgetResizable(true);  // 允许内容自动调整大小
        vlayoutFt->addWidget(scrollAreaFt);
        setLayout(vlayoutFt);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
}

void todoList::getTodoXmlFileZyInfo(const QDomElement &todoElement)
{
    QString flag = todoElement.firstChildElement("flag").text();
    int flag_I = flag.toInt();
    if(flag_I == 1)
    {
        QString id = todoElement.firstChildElement("id").text();
        QString date = todoElement.firstChildElement("date").text();
        QString time = todoElement.firstChildElement("time").text();
        QString title = todoElement.firstChildElement("title").text();
        QString notes = todoElement.firstChildElement("notes").text();
        QString flag = todoElement.firstChildElement("flag").text();
        layout_zyInfo = new QVBoxLayout;

        QLabel *l_date = new QLabel(QString("id:%1\nDateTime: %2 %3").arg(id, date, time));
        QLabel *l_notes = new QLabel(QString("title: %1\nnotes: %2").arg(title, notes));

        layout_zyInfo->addWidget(l_date);
        layout_zyInfo->addWidget(l_notes);
        if(flag.toInt() == 1)
        {

            QLabel *l_zy = new QLabel;
            QPixmap pixmap(":/tool/tool/transition.png");  // 使用资源文件路径
            l_zy->setPixmap(pixmap);
            l_zy->setScaledContents(true);
            l_zy->setFixedSize(pixmap.size());

            layout_zyInfo->addWidget(l_zy);
        }
        l_date->setStyleSheet("QLabel {"
                              "color: #333333;" /* 文本颜色 */
                              "font-size: 14px;" /* 字体大小 */
                              "font-weight: bold;" /* 字体粗细 */
                              "font-family: Arial;" /* 字体家族 */
                              "background-color: #f0f0f0;" /* 背景颜色 */
                              "border: 2px solid #1c7cd6;" /* 边框颜色 */
                              "border-radius: 5px;" /* 边框圆角 */
                              "padding: 10px;" /* 内边距 */
                              "}");

        l_notes->setStyleSheet("QLabel {"
                               "color: #333333;" /* 文本颜色 */
                               "font-size: 14px;" /* 字体大小 */
                               "font-weight: bold;" /* 字体粗细 */
                               "font-family: Arial;" /* 字体家族 */
                               "background-color: #f0f0f0;" /* 背景颜色 */
                               "border: 2px solid #1c7cd6;" /* 边框颜色 */
                               "border-radius: 5px;" /* 边框圆角 */
                               "padding: 10px;" /* 内边距 */
                               "}");

        QWidget *widget = new QWidget;
        widget->setLayout(layout_zyInfo);
        widget->setStyleSheet("QWidget {"
                              "border: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #1c7cd6, stop:1 #ff5722);" /* 渐变边框 */
                              "border-radius: 5px;" /* 边框圆角 */
                              "background-color: #f0f0f0;" /* 背景颜色 */
                              "}");
        flowLayoutZy->addWidget(widget);

        centralWidgetZy->setLayout(flowLayoutZy);
        centralWidgetZy->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);  // 确保流布局内部控件的大小
        scrollAreaZy->setWidget(centralWidgetZy);
        scrollAreaZy->setWidgetResizable(true);  // 允许内容自动调整大小
        vlayoutZy->addWidget(scrollAreaZy);
        setLayout(vlayoutZy);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
}

void todoList::getTodoXmlFileFsInfo(const QDomElement &todoElement)
{
    QString date = todoElement.firstChildElement("date").text();
    QDate date_D = QDate::fromString(date, "yyyy-MM-dd");
    QString time = todoElement.firstChildElement("time").text();
    QTime time_T = QTime::fromString(time, "HH:mm");
    QDateTime datetime(date_D, time_T);
    if(datetime < QDateTime::currentDateTime())
    {
        QString id = todoElement.firstChildElement("id").text();
        QString date = todoElement.firstChildElement("date").text();
        QString time = todoElement.firstChildElement("time").text();
        QString title = todoElement.firstChildElement("title").text();
        QString notes = todoElement.firstChildElement("notes").text();
        QString flag = todoElement.firstChildElement("flag").text();
        layout_fsInfo = new QVBoxLayout;

        QLabel *l_date = new QLabel(QString("id:%1\nDateTime: %2 %3").arg(id, date, time));
        QLabel *l_notes = new QLabel(QString("title: %1\nnotes: %2").arg(title, notes));

        layout_fsInfo->addWidget(l_date);
        layout_fsInfo->addWidget(l_notes);
        if(flag.toInt() == 1)
        {

            QLabel *l_zy = new QLabel;
            QPixmap pixmap(":/tool/tool/transition.png");  // 使用资源文件路径
            l_zy->setPixmap(pixmap);
            l_zy->setScaledContents(true);
            l_zy->setFixedSize(pixmap.size());

            layout_fsInfo->addWidget(l_zy);
        }
        l_date->setStyleSheet("QLabel {"
                              "color: #333333;" /* 文本颜色 */
                              "font-size: 14px;" /* 字体大小 */
                              "font-weight: bold;" /* 字体粗细 */
                              "font-family: Arial;" /* 字体家族 */
                              "background-color: #f0f0f0;" /* 背景颜色 */
                              "border: 2px solid #1c7cd6;" /* 边框颜色 */
                              "border-radius: 5px;" /* 边框圆角 */
                              "padding: 10px;" /* 内边距 */
                              "}");

        l_notes->setStyleSheet("QLabel {"
                               "color: #333333;" /* 文本颜色 */
                               "font-size: 14px;" /* 字体大小 */
                               "font-weight: bold;" /* 字体粗细 */
                               "font-family: Arial;" /* 字体家族 */
                               "background-color: #f0f0f0;" /* 背景颜色 */
                               "border: 2px solid #1c7cd6;" /* 边框颜色 */
                               "border-radius: 5px;" /* 边框圆角 */
                               "padding: 10px;" /* 内边距 */
                               "}");

        QWidget *widget = new QWidget;
        widget->setLayout(layout_fsInfo);
        widget->setStyleSheet("QWidget {"
                              "border: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #1c7cd6, stop:1 #ff5722);" /* 渐变边框 */
                              "border-radius: 5px;" /* 边框圆角 */
                              "background-color: #f0f0f0;" /* 背景颜色 */
                              "}");
        flowLayoutFs->addWidget(widget);

        centralWidgetFs->setLayout(flowLayoutFs);
        centralWidgetFs->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);  // 确保流布局内部控件的大小
        scrollAreaFs->setWidget(centralWidgetFs);
        scrollAreaFs->setWidgetResizable(true);  // 允许内容自动调整大小
        vlayoutFs->addWidget(scrollAreaFs);
        setLayout(vlayoutFs);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
}
