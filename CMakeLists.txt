project( gilligan )
cmake_minimum_required( VERSION 2.8 )

find_package( Qt4 REQUIRED )
set( QT_USE_QTNETWORK True )
set( CMAKE_AUTOMOC True )

set( gilligan_srcs
	connectdialog.cxx conversationwidget.cxx mainwindow.cxx
        registrationwizard.cxx registrationwizard_private.cxx
	dcpcommandprocessor.cxx dcpconnection.cxx dcpmessage.cxx
	main.cxx utility.cxx
	)

include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )

find_package( KDE4 )
if( KDE4_FOUND )
	include_directories( ${KDE4_INCLUDES} )
	add_definitions( -DHAVE_KDE ${KDE_DEFINITIONS} )
	kde4_add_executable( gilligan ${gilligan_srcs} )
	target_link_libraries( gilligan Qt4::QtGui Qt4::QtNetwork
		${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} )
else( KDE4_FOUND )
	if( NOT APPLE )
		add_executable( gilligan ${gilligan_srcs} )
	else()
		add_executable( gilligan MACOSX_BUNDLE ${gilligan_srcs}
			${gilligan_moc_srcs} )
	endif()
	target_link_libraries( gilligan Qt4::QtGui Qt4::QtNetwork )
endif( KDE4_FOUND )
