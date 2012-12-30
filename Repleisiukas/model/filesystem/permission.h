#ifndef PERMISSION_H
#define PERMISSION_H

#include <QObject>
#include "../irepleisiukasscriptobject.h"

class Permission : public IRepleisiukasScriptObject
{
	Q_OBJECT
	public:
		explicit Permission(QScriptEngine *parent = 0);

		bool
			uR, uW, uX,
			gR, gW, gX,
			oR, oW, oX;

		int uid;
		int gid;
		QString user;
		QString group;

		static void registerMetaType(QScriptEngine *engine);

	public slots:
		QString toString();

	private:
		QString formatPermission(bool r, bool w, bool x);
};

#endif // PERMISSION_H
