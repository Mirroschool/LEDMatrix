import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

import '../models.dart';
import 'firmwares/firmwares_manager.dart';

const MODE_TITLES = {
  "canvas": "Піксель-арт",
  "animations": "Обрати анімацію",
  "clock": "Годинник",
  "stopwatch": "Секундомір",
  "creepingLine": "Рухомий рядок",
  "games": "Ігровий контроллер"
};

class MyHomePage extends StatefulWidget {
  MyHomePage({Key key, this.title}) : super(key: key);

  final String title;

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  String _ip, _title, _current_mode;
  Settings _settings = Settings();

  Widget currentBody;

  List<ListTile> createDrawerTiles(List<String> modes) {
    List<ListTile> tiles = List<ListTile>();
    int i = 0;

    for (String mode in MODE_TITLES.keys) {
      tiles.add(ListTile(
        title: Text(MODE_TITLES[mode]),
        enabled: modes.contains(mode),
        onTap: () {
          setState(() {
            _title = MODE_TITLES[mode];
            _current_mode = mode;
            print("changed pageI");
          });
        },
      ));
      i++;
    }

    return tiles;
  }

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
        Settings settings = await Settings.getSettings(_ip);
        prefs.setStringList("modes", this._settings.modes);
        prefs.setInt("width", this._settings.width);
        prefs.setInt("height", this._settings.height);

        setState(() {
          _settings = settings;
        });
      } on Exception catch (_) {
        print("Err! $_");
        prefs.remove("ip");

        Navigator.pushNamedAndRemoveUntil(
            context, '/login', ModalRoute.withName('/login'));
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(_title ?? ""),
      ),
      body: FirmwaresManager(
        mode: _current_mode,
      ),
      drawer: Drawer(
        child: ListView(
          // Important: Remove any padding from the ListView.
          padding: EdgeInsets.zero,
          children: <Widget>[
            DrawerHeader(
              child: Text('Width: ${_settings.width ?? 0}\n'
                  'Height: ${_settings.height ?? 0}'),
              decoration: BoxDecoration(
                color: Colors.blue,
              ),
            ),
            ...createDrawerTiles(_settings.modes ?? [])
          ],
        ),
      ),
    );
  } //
}
