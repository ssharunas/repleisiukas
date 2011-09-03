#ifndef JSHIGHLIGHTER_H
#define JSHIGHLIGHTER_H

#include <QSyntaxHighlighter>

 #include <QHash>
 #include <QTextCharFormat>

class JSHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT

	 public:
		 JSHighlighter(QTextDocument *parent = 0);

	 protected:
		 void highlightBlock(const QString &text);

	 private:
		 struct HighlightingRule
		 {
			 QRegExp pattern;
			 QTextCharFormat format;
		 };

		 QVector<HighlightingRule> highlightingRules;

		 QRegExp commentStartExpression;
		 QRegExp commentEndExpression;

		 QTextCharFormat keywordFormat;
		 QTextCharFormat singleLineCommentFormat;
		 QTextCharFormat multiLineCommentFormat;
                 QTextCharFormat multiLineStringFormat;
		 QTextCharFormat quotationFormat;
		 QTextCharFormat quotationFormat2;
		 QTextCharFormat functionFormat;
};

#endif // JSHIGHLIGHTER_H
