import React, { Component } from 'react';
import { StyleSheet, Text, View, Button, FlatList, InteractionManager, TouchableOpacity, Pressable } from 'react-native';
import { NativeModules, NativeEventEmitter } from 'react-native';

const KeyStrokeEmitter = new NativeEventEmitter(NativeModules.NativeModuleClass);
const VK_PAD_A = 0x5800;
const VK_PAD_B = 0x5801;
const XBOX_A = 0x000000c3;
const XBOX_B = 0x000000c4;


const init = () => {
    // Setup OnKeyDown hook
    NativeModules.NativeModuleClass.init();
}


var log = function (result) {
    console.warn(result);
};

var getCallback = function (prefix) {
    return function (result) {

        switch (result) {
            case VK_PAD_A:
            case XBOX_A:
            result = ' The A (Select) Key was pressed';
            break;
            case VK_PAD_B:
            case XBOX_B:
                result = 'The B (Back) Key was pressed';
            break;
        }
    
        log(prefix + result);
    };

};


class MyFlatList extends Component {

    state = {
        data: [
            { key: "Text 1" },
            { key: "Text 2" },
            { key: "Text 3" },
            { key: "Text 4" },
            { key: "Text 5" },
        ]
    }

    componentDidMount() {
        init();
        this.KeyStrokeEventSub = KeyStrokeEmitter.addListener('EmitKeystrokeEvent', getCallback('NativeModule.EmitKeystrokeEvent => '));
    }

    componentWillUnmount() {
        this.KeyStrokeEventSub.remove();
    }

    init() {
        NativeModules.NativeModuleClass.init();
    }

    mybuttonclick = (item) => {
        console.warn('click: ' + item.key);
    }

    onPressFunction = (item) => {
        console.warn('onPressFunction:' + item.key);
    }

    renderSeparator = () => {
        return (
            <View
                style={{
                    height: 1,
                    width: "100%",
                    backgroundColor: "#000",
                }}
            />
        );
    };

    render() {
        return (
            <View style={styles.container}>
                <FlatList
                    //horizontal={true}
                    data={this.state.data}
                    renderItem={({ item }) => {
                        return (
                            <Pressable onPress={() => this.onPressFunction(item)}>
                                <Text style={styles.item} onPress={() => this.mybuttonclick(item)}>{item.key}</Text>
                            </Pressable>);
                    }}
                    ItemSeparatorComponent={this.renderSeparator}
                />
            </View>
        );
    }

}

const styles = StyleSheet.create({
    container: {
        flex: 1
    },
    item: {
        padding: 10,
        fontSize: 18,
        height: 44,
    },
});

export { MyFlatList };
