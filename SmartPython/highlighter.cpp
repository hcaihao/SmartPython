#include "stdafx.h"
#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
	: QSyntaxHighlighter(parent)
{
	initialize();
}

void Highlighter::highlightBlock(const QString &text)
{
	highlightPythonBlock(text);
}

/////

void Highlighter::initialize()
{
	// Python keywords
	const QStringList keywords = {
		"and", "assert", "break", "class", "continue", "def",
		"del", "elif", "else", "except", "exec", "finally",
		"for", "from", "global", "if", "import", "in",
		"is", "lambda", "not", "or", "pass", "print",
		"raise", "return", "try", "while", "yield",
		"None", "True", "False"
	};

	// Python operators
	const QStringList operators = {
		"=",
		// Comparison
		"==", "!=", "<", "<=", ">", ">=",
		// Arithmetic
		"\\+", "-", "\\*", "/", "//", "\\%", "\\*\\*",
		// In-place
		"\\+=", "-=", "\\*=", "/=", "\\%=",
		// Bitwise
		"\\^", "\\|", "\\&", "\\~", ">>", "<<"
	};

	// Python braces
	const QStringList braces = {
		"\\{", "\\}", "\\(", "\\)", "\[", "\]"
	};

	//FormatMap STYLES;
	const FormatMap STYLES = {
		{ "keyword", Highlighter::format("blue") },
		{ "operator", Highlighter::format("red") },
		{ "brace", Highlighter::format("darkGray") },
		{ "defclass", Highlighter::format("black", "bold") },
		{ "string", Highlighter::format("magenta") },
		{ "string2", Highlighter::format("darkMagenta") },
		{ "comment", Highlighter::format("darkGreen", "italic") },
		{ "self", Highlighter::format("black", "italic") },
		{ "numbers", Highlighter::format("brown") }
	};


	// Multi-line strings (expression, flag, style)
	// FIXME: The triple-quotes in these two lines will mess up the
	// syntax highlighting from this point onward
	_triSingle = HighlightingRule("'''", 1, STYLES["string2"]);
	_triDouble = HighlightingRule("\"\"\"", 2, STYLES["string2"]);

	// Keyword, operator, and brace rules
	for (const QString &keyword : keywords)
	{
		QString pattern = QString("\\b%1\\b").arg(keyword);
		_pythonHighlightingRules += HighlightingRule(pattern, 0, STYLES["keyword"]);
	}

	for (const QString &pattern : operators)
		_pythonHighlightingRules += HighlightingRule(pattern, 0, STYLES["operator"]);

	for (const QString &pattern : braces)
		_pythonHighlightingRules += HighlightingRule(pattern, 0, STYLES["brace"]);

	// All other rules

	// 'self'
	_pythonHighlightingRules += HighlightingRule("\\bself\\b", 0, STYLES["self"]);

	// Double-quoted string, possibly containing escape sequences
	_pythonHighlightingRules += HighlightingRule("\"[^\"\\\]*(\\\\.[^\"\\\]*)*\"", 0, STYLES["string"]);
	// Single-quoted string, possibly containing escape sequences
	_pythonHighlightingRules += HighlightingRule("\"[^'\\\]*(\\\\.[^'\\\]*)*\"", 0, STYLES["string"]);

	// 'def' followed by an identifier
	_pythonHighlightingRules += HighlightingRule("\\bdef\\b\\s*(\\w+)", 1, STYLES["defclass"]);
	// 'class' followed by an identifier
	_pythonHighlightingRules += HighlightingRule("\\bclass\\b\\s*(\\w+)", 1, STYLES["defclass"]);

	// From '#' until a newline
	_pythonHighlightingRules += HighlightingRule("#[^\\n]*", 0, STYLES["comment"]);

	// Numeric literals
	_pythonHighlightingRules += HighlightingRule("\\b[+-]?[0-9]+[lL]?\\b", 0, STYLES["numbers"]);
	_pythonHighlightingRules += HighlightingRule("\\b[+-]?0[xX][0-9A-Fa-f]+[lL]?\\b", 0, STYLES["numbers"]);
	_pythonHighlightingRules += HighlightingRule("\\b[+-]?[0-9]+(?:\\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\\b", 0, STYLES["numbers"]);
}

void Highlighter::highlightPythonBlock(const QString &text)
{
	if (text.isEmpty())
		return;

	int index = -1;

	// Do other syntax formatting
	for (const auto &rule : _pythonHighlightingRules)
	{
		index = rule.pattern.indexIn(text, 0);

		// We actually want the index of the nth match
		while (index >= 0)
		{
			index = rule.pattern.pos(rule.matchIndex);
			int length = rule.pattern.cap(rule.matchIndex).length();
			if (length > 0)
			{
				setFormat(index, length, rule.format);
				index = rule.pattern.indexIn(text, index + length);
			}
		}
	}

	setCurrentBlockState(0);

	// Do multi-line strings
	bool in_multiline = matchMultiLine(text, _triSingle);
	if (!in_multiline)
		in_multiline = matchMultiLine(text, _triDouble);
}

// Return a QTextCharFormat with the given attributes.
QTextCharFormat Highlighter::format(const QString &colorName, const QString &style)
{
	QColor color;
	color.setNamedColor(colorName);

	QTextCharFormat format;
	format.setForeground(color);

	if (style.contains("bold"))
		format.setFontWeight(QFont::Bold);
	if (style.contains("italic"))
		format.setFontItalic(true);

	return format;
}

/*Do highlighting of multi-line strings. ``delimiter`` should be a
``QRegExp`` for triple-single-quotes or triple-double-quotes, and
``in_state`` should be a unique integer to represent the corresponding
state changes when inside those strings. Returns True if we're still
inside a multi-line string when this function is finished.
*/
bool Highlighter::matchMultiLine(const QString &text, const HighlightingRule &rule)
{
	int start, add, end, length;

	// If inside triple-single quotes, start at 0
	if (previousBlockState() == rule.matchIndex)
	{
		start = 0;
		add = 0;
	}
	// Otherwise, look for the delimiter on this line
	else
	{
		start = rule.pattern.indexIn(text);
		// Move past this match
		add = rule.pattern.matchedLength();
	}

	// As long as there's a delimiter match on this line...
	while (start >= 0)
	{
		// Look for the ending delimiter
		end = rule.pattern.indexIn(text, start + add);
		// Ending delimiter on this line?
		if (end >= add)
		{
			length = end - start + add + rule.pattern.matchedLength();
			setCurrentBlockState(0);
		}
		// No; multi-line string
		else
		{
			setCurrentBlockState(rule.matchIndex);
			length = text.length() - start + add;
		}

		// Apply formatting
		setFormat(start, length, rule.format);

		// Look for the next match
		start = rule.pattern.indexIn(text, start + length);
	}

	// Return True if still inside a multi-line string, False otherwise
	if (currentBlockState() == rule.matchIndex)
		return true;
	else
		return false;
}