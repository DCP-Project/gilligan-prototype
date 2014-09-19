project( gilligan )
cmake_minimum_required( VERSION 2.8 )

find_package( Qt4 REQUIRED )
set( QT_USE_QTNETWORK True )

set( gilligan_srcs
	connectdialog.cxx conversationwidget.cxx mainwindow.cxx
        registrationwizard.cxx registrationwizard_private.cxx
	dcpcommandprocessor.cxx dcpconnection.cxx dcpmessage.cxx
	main.cxx utility.cxx
	)

set( gilligan_headers
	connectdialog.h conversationwidget.h mainwindow.h registrationwizard.h
	registrationwizard_private.h
	dcpcommandprocessor.h dcpconnection.h dcpmessage.h
	utiltity.h
	)

set( gillian_moc_headers
	connectdialog.h conversationwidget.h mainwindow.h registrationwizard.h
	registrationwizard_private.h
	dcpcommandprocessor.h dcpconnection.h dcpmessage.h
	)

include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )
qt4_wrap_cpp( gilligan_moc_srcs ${gilligan_moc_headers} )

find_package( KDE4 )
if( KDE4_FOUND )
	include_directories( ${KDE4_INCLUDES} )
	add_definitions( -DHAVE_KDE )
	kde4_add_executable( gilligan ${gilligan_srcs} ${gilligan_moc_srcs} )
	target_link_libraries( gilligan ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} )
else( KDE4_FOUND )
	add_executable( gilligan ${gilligan_srcs} ${gilligan_moc_srcs} )
	target_link_libraries( gilligan ${QT_LIBRAIES} )
endif( KDE4_FOUND )

