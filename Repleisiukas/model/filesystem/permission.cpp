#include "permission.h"

Permission::Permission(QObject *parent) :
	QObject(parent),
	uR(false), uW(false), uX(false),
	gR(false), gW(false), gX(false),
	oR(false), oW(false), oX(false),
	uid(0), gid(0)
{

}

QString Permission::toString()
{
	return formatPermission(uR, uW, uX) +
			formatPermission(gR, gW, gX) +
			formatPermission(oR, oW, oX);
}

QString Permission::formatPermission(bool r, bool w, bool x)
{
	char result[4];

	result[0] = r ? 'r' : '-';
	result[1] = w ? 'w' : '-';
	result[2] = x ? 'x' : '-';
	result[3] = 0;

	return QString(result);
}
