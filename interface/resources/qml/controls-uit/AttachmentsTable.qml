import QtQuick 2.0
import controlsUit 1.0

AttachmentsTable {
    Component.onCompleted: {
        console.warn("warning: including controls-uit is deprecated! please use 'import controlsUit 1.0' instead");
    }
}