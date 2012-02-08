#ifndef PERMISSION_H
#define PERMISSION_H

#include <QObject>

class Permission : public QObject
{
		Q_OBJECT
	public:
		explicit Permission(QObject *parent = 0);

		bool
			uR, uW, uX,
			gR, gW, gX,
			oR, oW, oX;

		int uid;
		int gid;
		QString user;

		QString toString();

	private:
		QString formatPermission(bool r, bool w, bool x);
};

#endif // PERMISSION_H
