#include "todolist.h"
#include "ui_todolist.h"


todoList::todoList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::todoList)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);
    QDate curDate = QDate::currentDate();
    ui->dateEdit->setDate(curDate);
    QTime curTime = QTime::currentTime();
    ui->timeEdit->setTime(curTime);

    flowLayout = new FlowLayout(ui->page_all);
}

todoList::~todoList()
{
    delete ui;
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
}

void todoList::on_pb_future_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void todoList::on_pb_important_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void todoList::on_pb_all_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    // clearPage_allWidget();

    QString rootDir = QCoreApplication::applicationDirPath();

    // 构建相对于根目录的sta.ini文件路径
    QString xmlFilePath = QDir(rootDir).filePath("config/todo/todo.xml");

    updatePage_allWidget(xmlFilePath);
}

void todoList::on_pb_finish_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void todoList::on_pb_sure_clicked()
{
    QString rootDir = QCoreApplication::applicationDirPath();

    // 构建相对于根目录的sta.ini文件路径
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

void todoList::updatePage_allWidget(const QString &file)
{
    clearPage_allWidget();
    loadTodoXmlFileInfo(file);
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

void todoList::loadTodoXmlFileInfo(const QString &file)
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
    for (int i = 0; i < todos.count(); ++i) {
        QDomElement todoElement = todos.at(i).toElement();
        getTodoXmlFileInfo(todoElement); // 显示 TODO 的详细信息
    }
}

void todoList::getTodoXmlFileInfo(const QDomElement &todoElement)
{
    layout_info = new QVBoxLayout(ui->page_all);
    QString id = todoElement.firstChildElement("id").text();
    QString date = todoElement.firstChildElement("date").text();
    QString time = todoElement.firstChildElement("time").text();
    QString title = todoElement.firstChildElement("title").text();
    QString notes = todoElement.firstChildElement("notes").text();
    QLabel *label_date = new QLabel(QString("ID: %1\nDate: %2\tTime: %3").arg(id, date, time), ui->page_all);
    QLabel *label_notes = new QLabel(QString("title: %1\nnotes: %2").arg(title, notes));
    layout_info->addWidget(label_date);
    layout_info->addWidget(label_notes);

    QWidget *widget = new QWidget;
    widget->setLayout(layout_info);
    flowLayout->addWidget(widget);
    setLayout(flowLayout);
}
