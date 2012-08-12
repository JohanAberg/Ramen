# Copyright (c) 2012 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import sys
from code import InteractiveConsole as _InteractiveConsole

import logging

try:
    from cStringIO import StringIO
except ImportError:
    from StringIO import StringIO

import PySide.QtCore as QtCore
import PySide.QtGui as QtGui

from ramen import r_app

class _console_interpreter(_InteractiveConsole):
    def __init__( self, ui, locals = None):
        _InteractiveConsole.__init__( self, locals)
        self.__ui = ui

    def write( self, data):
        if data:
            if data[-1] == "\n":
                data = data[:-1]
            self.__ui.appendPlainText(data)

    def runsource( self,source, filename="<input>", symbol="single"):
        old_stdout = sys.stdout
        old_stderr = sys.stderr
        sys.stdout = sys.stderr = collector = StringIO()

        try:
            more = _InteractiveConsole.runsource( self, source, filename, symbol)
        finally:
            if sys.stdout is collector:
                sys.stdout = old_stdout
            if sys.stderr is collector:
                sys.stderr = old_stderr
        self.write( collector.getvalue())
        return more

class _console_text_area( QtGui.QPlainTextEdit):
    def __init__( self, parent = None):
        super( _console_text_area, self).__init__( parent)
        self.setReadOnly( True)
        self.setUndoRedoEnabled( False)
        self.setMaximumBlockCount( 5000)
        fmt = QtGui.QTextCharFormat()
        fmt.setFontFixedPitch( True)
        self.setCurrentCharFormat( fmt)
        self.setTabStopWidth( 40)
        self.clear_all()
        self.__command = ""
        self.__history = []
        self.__history_pos = 0
        self.__interpreter = _console_interpreter( self, r_app().main_namespace())
        self.installEventFilter( self)

    def clear_all( self):
        self.clear()
        self.appendPlainText( ">>> ")

    def keyPressEvent( self, event):
        if event.key() == QtCore.Qt.Key_Enter or event.key() == QtCore.Qt.Key_Return:
            logging.debug( 'Command = %s' % self.__command)
            more = self.__interpreter.push( self.__command)
            self.__command = ""

            if more:
                self.insertPlainText( "\n... ")
            else:
                self.insertPlainText( "\n>>> ")

            self.verticalScrollBar().setValue( self.verticalScrollBar().maximum())

        # TODO: handle more keys here
        elif event.key() == QtCore.Qt.Key_Backspace or event.key() == QtCore.Qt.Key_Delete:
            logging.debug( 'delete pressed')
            self.textCursor().deletePreviousChar()
            self.__command = self.__command[:-1]
        else:
            self.insertPlainText( event.text())
            self.__command += event.text()

        event.accept()

    def eventFilter( self, obj, event):
        if event.type() == QtCore.QEvent.KeyPress:
            if event.key() == QtCore.Qt.Key_Tab:
                logging.debug( 'tab pressed')
                self.insertPlainText( "\t")
                self.__command += "\t"
                return True

        return False

class python_console_widget( QtGui.QWidget):
    def __init__( self, parent = None):
        super( python_console_widget, self).__init__( parent)
        layout = QtGui.QVBoxLayout()
        self.__text_area = _console_text_area()
        layout.addWidget( self.__text_area)
        self.setLayout( layout)

"""
class python_console_widget( QtGui.QWidget):

    def __init__( self,parent=None,locals=None):
        super( python_console_widget,self).__init__(parent)
        self.ui = _QPythonConsoleUI(self)
        self.interpreter = _QPythonConsoleInterpreter(self.ui,locals)
        self.ui.input.returnPressed.connect(self._on_enter_line)

    def _on_enter_line(self):
        line = self.ui.input.text()
        self.ui.input.setText("")
        self.interpreter.write(self.ui.prompt.text() + line)
        more = self.interpreter.push(line)
        if line:
            self.history.append(line)
            self.history_pos = len(self.history)
            while len(self.history) > 100:
                self.history = self.history[1:]
                self.history_pos -= 1
        if more:
            self.ui.prompt.setText("... ")
        else:
            self.ui.prompt.setText(">>> ")

    def eventFilter(self,obj,event):
        if event.type() == QtCore.QEvent.KeyPress:
            if event.key() == QtCore.Qt.Key_Up:
                self.go_history(-1)
            elif event.key() == QtCore.Qt.Key_Down:
                self.go_history(1)
        return False

    def go_history(self,offset):
        if offset < 0:
            self.history_pos = max(0,self.history_pos + offset)
        elif offset > 0:
            self.history_pos = min(len(self.history),self.history_pos + offset)
        try:
            line = self.history[self.history_pos]
        except IndexError:
            line = ""
        self.ui.input.setText(line)
"""
