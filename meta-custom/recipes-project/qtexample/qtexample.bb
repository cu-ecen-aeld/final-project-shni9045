SUMMARY = "QT Example Recipe"
LICENSE = "CLOSED"

SRC_URI = "file://qtexample.pro \
           file://qtexample.cpp"

DEPENDS += "qtbase"
RDEPENDS_${PN} += "qtwayland"


#FILES_${PN} += "${bindir}/qtexample"

S = "${WORKDIR}"


inherit qmake5

#do_install () {

#    install -d ${D}${bindir}
#    install -m 0755 ${S}/qtexample ${D}${bindir}/	

#}

