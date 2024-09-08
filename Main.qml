import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import App
import "ui"

Window {
    id: root
    width: Calculator.settings.width
    minimumWidth: 400
    height: Calculator.settings.height
    minimumHeight: 580
    visible: true
    title: qsTr("Calc")
    x: Calculator.settings.leftPos
    y: Calculator.settings.topPos

    property var buttons: [
        {
            "name": "AC",
            "color": "#f2aab1"
        },
        {
            "name": "del"
        },
        {
            "name": "*"
        },
        {
            "name": "/"
        },
        {
            "name": "7"
        },
        {
            "name": "8"
        },
        {
            "name": "9"
        },
        {
            "name": "-"
        },
        {
            "name": "4"
        },
        {
            "name": "5"
        },
        {
            "name": "6"
        },
        {
            "name": "+"
        },
        {
            "name": "1"
        },
        {
            "name": "2"
        },
        {
            "name": "3"
        },
        {
            "name": "=",
            "rows": 2
        },
        {
            "name": "0",
            "cols": 2
        },
        {
            "name": "."
        }
    ]

    ColumnLayout {
        id: column
        spacing: 10
        width: parent.width - 40
        height: parent.height - 20
        anchors.centerIn: parent
        Item {
            id: inputWindow
            Layout.preferredHeight: 50
            Layout.preferredWidth: parent.width

            property bool isError: false

            Rectangle
            {
                width: parent.width
                height: parent.height
                border {
                    width: 2
                    color: inputWindow.isError ? "red" : "black"
                }
                TextInput
                {
                  id: input
                  width: parent.width - 20
                  height: parent.height - 20
                  anchors.centerIn: parent
                  color: inputWindow.isError ? "red" : "black"
                  text: "0"
                  font: Qt.font({
                                    family: "Inter",
                                    pixelSize: 18,
                                    letterSpacing: 0,
                                    weight: Font.Normal
                                })
                  leftPadding: 20
                  rightPadding: 20
                  verticalAlignment: Text.AlignVCenter
                  horizontalAlignment: Text.AlignRight

                  onTextChanged: {
                      if(text === "") {
                          text = "0";
                          inputWindow.isError = false;
                      }
                  }

                }
            }
        }

        Item {
            id: controls
            Layout.preferredHeight: 400
            Layout.preferredWidth: parent.width

            GridLayout {
                id: grid
                width: parent.width
                height: parent.height
                columns: 4
                rows: 5
                Repeater {
                    model: root.buttons
                    delegate: CalcButton {
                        id: btn
                        Layout.preferredWidth: {
                            let postWidth = (btn.Layout.columnSpan - 1) * grid.rowSpacing;
                            return (grid.width - (grid.columns - 1) * grid.rowSpacing) /
                                    grid.columns * btn.Layout.columnSpan + postWidth;
                        }
                        Layout.preferredHeight: {
                            let postHeight = (btn.Layout.rowSpan - 1) * grid.columnSpacing;
                            return (grid.height - (grid.rows - 1) * grid.columnSpacing) /
                                    grid.rows * btn.Layout.rowSpan + postHeight;
                        }
                        title: modelData.name
                        Layout.columnSpan: modelData.cols ? modelData.cols : 1
                        Layout.rowSpan: modelData.rows ? modelData.rows : 1

                        function block() {
                            btn.timer.running = true;
                            btn.enabled = false;
                        }

                        onClicked: {
                            switch(modelData.name) {
                            case "del":
                                let str = String(input.text);
                                input.text = str.slice(0, -1);
                                break;
                            case "AC":
                                input.text = "0";
                                Calculator.request = "";
                                inputWindow.isError = false;
                                break;
                            case "=":
                                Calculator.request = input.text;
                                break;
                            default:
                                input.text === "0" && modelData.name !== "."
                                    ? input.text = modelData.name
                                    : input.text += modelData.name;
                            }
                        }

                        Component.onCompleted: {
                            Calculator.requestChanged.connect(btn.block);
                        }
                    }

                }
            }
        }

        Item {
            id: info
            Layout.preferredHeight: 50
            Layout.preferredWidth: parent.width
            Row{
                id: infoRow
                width: parent.width
                height: parent.height / 2
                spacing: 0
                Item {
                    height: parent.height
                    width: parent.width / 3 * 2 - 20
                    Row{
                        width: parent.width
                        height: parent.height
                        Text{
                            text: "Кол-во запросов в обработке - "
                        }
                        Text{
                            text: "0"
                            signal sizeChanged(size: int)
                            onSizeChanged: function(size) {
                                text = size;
                            }

                            Component.onCompleted: {
                                Calculator.reqsQueue.sizeChanged.connect(sizeChanged)
                            }
                        }
                    }
                }
                Item {
                    height: parent.height
                    width: parent.width / 3
                    Row{
                        width: parent.width
                        height: parent.height
                        Text{
                            text: "Кол-во результатов - "
                        }
                        Text{
                            text: "0"
                            signal sizeChanged(size: int)
                            onSizeChanged: function(size) {
                                text = size;
                            }

                            Component.onCompleted: {
                                Calculator.results.sizeChanged.connect(sizeChanged)
                            }
                        }
                    }
                }
            }
            Row {
                id: durationSettings
                anchors {
                    top: infoRow.bottom
                }
                width: parent.width
                height: parent.height / 2
                Text{
                    text: "Настройка времени обработки запроса "
                }

                Rectangle
                {
                    width: 80
                    height: 20
                    border {
                        width: 2
                        color: "grey"
                    }
                    TextInput
                    {
                      width: parent.width
                      height: parent.height
                      anchors.centerIn: parent
                      color: "black"
                      font: Qt.font({
                                        family: "Inter",
                                        pixelSize: 16,
                                        letterSpacing: 0,
                                        weight: Font.Normal
                                    })
                      leftPadding: 10
                      rightPadding: 10
                      verticalAlignment: Text.AlignVCenter
                      horizontalAlignment: Text.AlignHCenter

                      onTextChanged: {
                          Calculator.settings.duration = Number(text);
                      }

                      Component.onCompleted: {
                        text = Calculator.settings.duration;
                      }

                    }
                }
            }
        }

        Item {
            id: outputWindow
            Layout.preferredHeight: parent.height - info.height -
                                    inputWindow.height - controls.height -
                                    column.spacing - 20
            Layout.preferredWidth: parent.width

            ScrollView
            {
                width: parent.width
                height: parent.height
                background: Rectangle {
                    border {
                        width: 2
                        color: "#dedde4"
                    }
                }
                TextArea
                {
                  id: output
                  color: "black"
                  textFormat: Text.RichText
                  font: Qt.font({
                                    pixelSize: 16,
                                    letterSpacing: 0,
                                    weight: Font.Normal
                                })
                  leftPadding: 20
                  rightPadding: 20
                  verticalAlignment: Text.AlignTop

                  signal requestProcessed(req: string)
                  signal successResultGiven(value: string)
                  signal errorResultGiven(value: string)

                  onRequestProcessed: function(str) {
                      text = "<p style='color: green'>Запрос: " + str + "</p>"+ text;
                  }

                  onSuccessResultGiven: function(str) {
                    text = "<p style='color: blue'>Ответ: " + str + "</p>"+ text;
                      inputWindow.isError = false;
                  }

                  onErrorResultGiven: function(str) {
                    text = "<p style='color: red'>Ответ: " + str + "</p>"+ text;
                      inputWindow.isError = true;
                  }

                  Component.onCompleted: {
                    Calculator.reqsQueue.requestAdopted.connect(requestProcessed);
                    Calculator.results.success.connect(successResultGiven);
                    Calculator.results.error.connect(errorResultGiven);
                  }

                }
            }
        }

    }

    Component.onDestruction: {
        Calculator.settings.leftPos = x;
        Calculator.settings.topPos = y;
        Calculator.settings.width = width;
        Calculator.settings.height = height;
    }

}
