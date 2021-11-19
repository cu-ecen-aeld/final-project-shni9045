SUMMARY = "QT Example Recipe"
LICENSE = "CLOSED"

SRC_URI = "file://qtexample.pro \
           file://values.h \
           file://window.h \
           file://data-provider.h \
           file://window.cpp \
           file://values.cpp \
           file://main.cpp \
           file://data-provider.cpp"

DEPENDS += "qtbase"
RDEPENDS_${PN} += "qtwayland"

FILES_${PN} += "${bindir}/qtexample"


S = "${WORKDIR}"


#FILES_${PN} = "${datadir} ${bindir} ${systemd_unitdir}"
#FILES_${PN}-dbg = "${datadir}/${PN}/.debug"
#FILES_${PN}-dev = "/usr/src"

do_install:append () {

    install -d ${D}${bindir}
    install -m 0755 qtexample ${D}${bindir}/	

}



inherit qmake5
