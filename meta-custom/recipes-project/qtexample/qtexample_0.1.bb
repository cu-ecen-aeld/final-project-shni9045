SUMMARY = "QT Example Recipe"
LICENSE = "CLOSED"

SRC_URI = "file://qtexample.pro \
           file://qtexample.cpp"

DEPENDS += "qtbase"
RDEPENDS_${PN} += "qtwayland"

S = "${WORKDIR}"


inherit qmake5
