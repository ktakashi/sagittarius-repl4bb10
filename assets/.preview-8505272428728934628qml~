// Application with the Sheet project template
import bb.cascades 1.0

Page {
    attachedObjects: [
        // sheet is not visible still, prepare it and append to the attachedObjects
        // attachedObjects property is a storage for objects which are not supposed to be visible on scene like data models etc.
        Sheet {
            id: commentSheet
            // sheet GUI appearence component is defined in external CommentSheet.qml file
            content: CommentSheet {
                id: sheetContent
            }
        }
    ]
    content: Container {
        ListView {
            id: listView
            dataModel: ArrayDataModel {
                id: myListModel
            }
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0
            }
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill

            listItemComponents: [
                ListItemComponent {
                    // define component which will represent list item GUI appearence
                    StandardListItem {
                        title: ListItemData.title
                        description: ListItemData.comment
                    }
                }
            ]
            contextActions: [
                ActionSet {
                    // put context menu actions here
                    title: qsTr("Comment actions")
                    subtitle: qsTr("Set of the useful things to do with comments")
                    ActionItem {
                        title: qsTr("Edit")
                        onTriggered: {
                            // define action handler here
                            var data = listView.dataModel.value(listView.activeItem);
                            sheetContent.editComment(data);
                            commentSheet.open();

                        }
                    }
                    ActionItem {
                        title: qsTr("Delete")
                        onTriggered: {
                            // define action handler here
                            //console.log ("deleting item index: " + listView.activeItem);
                            listView.dataModel.removeAt(listView.activeItem)
                        }
                    }
                }
            ]
            property int activeItem: -1
            onActivationChanged: {
                if(active) activeItem = indexPath[0]
            }
        }
        Button {
            id: btAppendComment
            text: qsTr("Add comment")
            onClicked: {
                // when a button is clicked, the sheet is shown
                sheetContent.addComment();
                commentSheet.open();
            }
        }
    }
    function saveEdits(ok) {
        if(ok) {
            // when sheet is closed with success, append new data to the data model
            if(sheetContent.isAppend) {
                listView.dataModel.append({title: sheetContent.title, comment: sheetContent.comment});
            }
            else {
                var item_index = listView.activeItem;
                listView.dataModel.replace(item_index, {title: sheetContent.title, comment: sheetContent.comment});
            }
        }
        commentSheet.close();
    }
    onCreationCompleted: {
        // this slot is called when declarative scene is created
        // write post creation initialization here
        console.log("Page - onCreationCompleted()")

        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;

        // connect the sheet done signal to the handler slot
        sheetContent.done.connect(saveEdits)
        listView.dataModel.append({title: qsTr("Sample Title"), comment: qsTr("no comment")});
    }
}
