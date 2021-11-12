
SUMMARY = "Simple Qt5 Quick application"
SECTION = "examples"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

# I want to make sure these get installed too.
DEPENDS += "qtbase qtdeclarative qtquickcontrols2"

PV = "1.0+git${SRCPV}"
SRC_URI = "git://git@github.com/cu-ecen-aeld/final-project-chth2844.git;protocol=ssh;branch=main"
SRCREV = "${AUTOREV}"

S = "${WORKDIR}/git"


FILES_${PN} += "${bindir}/shri_gui_test"


require recipes-qt/qt5/qt5.inc

#do_configure () {
#	:
#}

#do_compile () {
#	oe_runmake
#}	

do_install() {
      install -d ${D}${bindir}
      install -m 0755 ${S}/Basic_Quick/shri_gui_test ${D}${bindir}/
}
