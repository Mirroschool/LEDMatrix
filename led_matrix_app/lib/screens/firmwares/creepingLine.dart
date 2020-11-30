import 'package:flutter/material.dart';

import 'dart:typed_data';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:dio/dio.dart';

Uint8List utf8ToCP1251(String sourceString) {
  var convertedBytes = Uint8List.fromList([33, 34, 35, 36, 255]);//List<int>();
  sourceString.runes.forEach((int rune) {
    var character = new String.fromCharCode(rune);
    print(character);
  });
  return convertedBytes;
}

class CreepingLine extends StatefulWidget {
  @override
  _CreepingLineState createState() => _CreepingLineState();
}

class _CreepingLineState extends State<CreepingLine> {
  String _ip;

  Future<Response> setText(String text) async {
    var uri = 'http://$_ip/modes/';

    print(text);

    FormData formData = FormData.fromMap({
      "mode_id": "1",
      "text": text,
    });
    var response = await Dio().post(uri, data: formData);

    return response;
  }

  @override
  void initState() {
    super.initState();
    _loadSettings();
  }

  _loadSettings() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    _ip = (prefs.getString('ip') ?? "");
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      child: SizedBox(
        width: double.infinity,
        // height: double.infinity,
        child: RaisedButton(
          onPressed: () async {
            await setText("аку");
          },
        ),
      ),
    );
  }
}
