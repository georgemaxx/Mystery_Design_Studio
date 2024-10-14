#include "srceditor.h"
#include "mainwindow.h"
#include <QPainter>
#include <QTextBlock>
#include <QDebug>

extern MainWindow *w;

//![constructor]

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    connect(this,
            SIGNAL( textChanged() ),
            this,
            SLOT( text_changed_event() )
            );

    connect(this,
            SIGNAL( cursorPositionChanged() ),
            this,
            SLOT( cursor_position_changed_event() )
            );

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

//![constructor]

//![extraAreaWidth]

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    // 10是数字前面的宽度
    // 个位数9的宽度，十位数2个9的宽度，百位数是3个9的宽度，以此类推
    int space = 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

//![extraAreaWidth]

//![slotUpdateExtraAreaWidth]

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

//![slotUpdateExtraAreaWidth]

//![slotUpdateRequest]

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

//![slotUpdateRequest]

//![resizeEvent]

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

//![resizeEvent]

void CodeEditor::text_changed_event()
{
//    qDebug() << "text changed";
//    qDebug() << "before: " << w->openFlag;
    if (w->openFlag == 0)
    {
        w->openFlag = 1;
    }
    else if (w->openFlag == 1)
    {
        int curId = w->cenTabWidget->currentIndex();
        QString curTabText = w->cenTabWidget->tabText(curId);
        if (!curTabText.contains("*"))
        {
            QString newCurTabText = curTabText + "*";
            w->cenTabWidget->setTabText(curId, newCurTabText);
        }
        w->openFlag = 2;
    }
    else
    {
        // Nothing to do
    }
//    qDebug() << "after: " << w->openFlag;
}

void CodeEditor::cursor_position_changed_event()
{
#if 0
    qDebug() << "cursor position changed";
    qDebug() << w->openFlag;
    if (w->openFlag == 0 || w->openFlag == 1)
    {

    }
    else
    {
        int curId = w->cenTabWidget->currentIndex();
        QString curTabText = w->cenTabWidget->tabText(curId);
        QString newCurTabText = curTabText + "*";
        w->cenTabWidget->setTabText(curId, newCurTabText);
        w->openFlag = 1;
    }

    w->openFlag = 2;
#endif
}

//![cursorPositionChanged]

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::gray).lighter(150);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

//![cursorPositionChanged]

//![extraAreaPaintEvent_0]

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::transparent);

//![extraAreaPaintEvent_0]

//![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
//![extraAreaPaintEvent_1]

//![extraAreaPaintEvent_2]
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
//![extraAreaPaintEvent_2]
