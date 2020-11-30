import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';
import '../models.dart';



class Landing extends StatefulWidget {
  @override
  _LandingState createState() => _LandingState();
}

class _LandingState extends State<Landing> {
  String _ip = "";

  @override
  void initState() {
    super.initState();
    _loadSettings();
  }

  _loadSettings() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    _ip = (prefs.getString('ip') ?? "");

    if (_ip == "") {
      Navigator.pushNamedAndRemoveUntil(
          context, '/login', ModalRoute.withName('/login'));
    } else {
      try {
        var settings = await Settings.getSettings(_ip);

        prefs.setStringList("modes", settings.modes);
        prefs.setInt("width", settings.width);
        prefs.setInt("height", settings.height);

      } on Exception catch (_) {
        print("Error! $_");
        Navigator.pushNamedAndRemoveUntil(
            context, '/login', ModalRoute.withName('/login'));
        return;
      }

      Navigator.pushNamedAndRemoveUntil(
          context, '/home', ModalRoute.withName('/home'));
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(body: Center(child: CircularProgressIndicator()));
  }
}
