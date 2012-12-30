#include "permission.h"
#include "../scriptvalueconverttemplate.h"

Q_DECLARE_METATYPE(Permission*)

Permission::Permission(QScriptEngine *parent) :
	IRepleisiukasScriptObject(parent),
	uR(false), uW(false), uX(false),
	gR(false), gW(false), gX(false),
	oR(false), oW(false), oX(false),
	uid(0), gid(0)
{

}

void Permission::registerMetaType(QScriptEngine *engine)
{
	qScriptRegisterMetaType<Permission*>(engine, &convertToScriptValue<Permission>, &convertFromScriptValue<Permission>);
}

QString Permission::toString()
{
	return formatPermission(uR, uW, uX) +
			formatPermission(gR, gW, gX) +
			formatPermission(oR, oW, oX) +
			QString(" %1(%2) %3(%4)").arg(user).arg(uid).arg(group).arg(gid);
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
