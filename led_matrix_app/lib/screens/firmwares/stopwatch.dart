import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'package:shared_preferences/shared_preferences.dart';

class Stopwatch extends StatefulWidget {
  @override
  _StopwatchState createState() => _StopwatchState();
}

class _StopwatchState extends State<Stopwatch> {
  String _ip;

  Future<http.Response> resetStopwatch() async {
    var uri = Uri.parse('http://$_ip/modes/');
    var request = http.MultipartRequest('POST', uri)..fields['mode_id'] = "2";

    var streamedResponse = await request.send();
    var response = await http.Response.fromStream(streamedResponse);
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
          child: Text("Обнулити секундомір"),
          onPressed: () async {
            await resetStopwatch();
          },
        ),
      ),
    );
  }
}
