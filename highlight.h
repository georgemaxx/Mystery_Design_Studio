#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H

#include "common.h"

class CSyntaxHighlight : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    CSyntaxHighlight(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override; // replement highlightBlock method

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;

};


#endif // HIGHLIGHT_H
