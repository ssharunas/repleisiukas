#include <QtGui>
#include <QColor>
#include "jshighlighter.h"

JSHighlighter::JSHighlighter(QTextDocument *parent)
	 : QSyntaxHighlighter(parent)
 {
	 HighlightingRule rule;

	 singleLineCommentFormat.setForeground(Qt::darkGray);
	 rule.pattern = QRegExp("//[^\n]*");
	 rule.format = singleLineCommentFormat;
	 highlightingRules.append(rule);

	 multiLineCommentFormat.setForeground(Qt::darkGray);

	 quotationFormat.setForeground(Qt::darkGreen);
	 rule.pattern = QRegExp("\"[^\"]*\"");
	 rule.format = quotationFormat;
	 highlightingRules.append(rule);

	 quotationFormat2.setForeground(QColor(0x33, 0x70, 0x13));
	 rule.pattern = QRegExp("\'[^\']*\'");
	 rule.format = quotationFormat2;
	 highlightingRules.append(rule);

	 functionFormat.setFontItalic(true);
	 functionFormat.setForeground(Qt::blue);
	 rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	 rule.format = functionFormat;
	 highlightingRules.append(rule);

	 commentStartExpression = QRegExp("/\\*");
	 commentEndExpression = QRegExp("\\*/");

	 keywordFormat.setForeground(Qt::darkBlue);
	 keywordFormat.setFontWeight(QFont::Bold);

         QTextCharFormat format;
         format.setBackground(QColor(0xff,0xc5,0x7d));
         rule.pattern = QRegExp("/[^/]*/");
         rule.format = format;
         highlightingRules.append(rule);

	 QStringList keywordPatterns;

	 keywordPatterns << "\\bin\\b" << "\\bif\\b" << "\\bfor\\b"
					 << "\\bwhile\\b" << "\\bfinally\\b" << "\\bvar\\b"
					 << "\\bnew\\b" << "\\bfunction\\b" << "\\bdo\\b"
					 << "\\breturn\\b" << "\\bvoid\\b" << "\\belse\\b"
					 << "\\bbreak\\b" << "\\bcatch\\b" << "\\binstanceof\\b"
					 << "\\bwith\\b" << "\\bthrow\\b" << "\\bcase\\b"
					 << "\\bdefault\\b" << "\\btry\\b" << "\\bthis\\b"
					 << "\\bswitch\\b" << "\\bcontinue\\b" << "\\btypeof\\b"
					 << "\\bdelete\\b";
	 foreach (QString pattern, keywordPatterns) {
		 rule.pattern = QRegExp(pattern);
		 rule.format = keywordFormat;
		 highlightingRules.append(rule);
	 }

	 keywordPatterns.clear();

	 QTextCharFormat var;
	 var.setForeground(Qt::darkYellow);
	 var.setFontWeight(QFont::Bold);

	 keywordPatterns << "\\btrue\\b" << "\\bfalse\\b" << "\\bnull\\b";
	 foreach (QString pattern, keywordPatterns) {
		 rule.pattern = QRegExp(pattern);
		 rule.format = var;
		 highlightingRules.append(rule);
	 }

 }

 void JSHighlighter::highlightBlock(const QString &text)
 {
	 foreach (HighlightingRule rule, highlightingRules) {
		 QRegExp expression(rule.pattern);
		 int index = text.indexOf(expression);
		 while (index >= 0) {
			 int length = expression.matchedLength();
			 setFormat(index, length, rule.format);
			 index = text.indexOf(expression, index + length);
		 }
	 }
	 setCurrentBlockState(0);

	 int startIndex = 0;
	 if (previousBlockState() != 1)
		 startIndex = text.indexOf(commentStartExpression);

	 while (startIndex >= 0) {
		 int endIndex = text.indexOf(commentEndExpression, startIndex);
		 int commentLength;
		 if (endIndex == -1) {
			 setCurrentBlockState(1);
			 commentLength = text.length() - startIndex;
		 } else {
			 commentLength = endIndex - startIndex
							 + commentEndExpression.matchedLength();
		 }
		 setFormat(startIndex, commentLength, multiLineCommentFormat);
		 startIndex = text.indexOf(commentStartExpression,
												 startIndex + commentLength);
	 }
 }
