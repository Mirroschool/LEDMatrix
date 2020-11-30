import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

class Login extends StatefulWidget {
  @override
  _LoginState createState() => _LoginState();
}

class _LoginState extends State<Login> {
  final GlobalKey<ScaffoldState> _scaffoldKey = GlobalKey<ScaffoldState>();

  String _ip = "192.168.1.137";

  final GlobalKey<FormState> _formKey = GlobalKey<FormState>();

  void showInSnackBar(String value) {
    _scaffoldKey.currentState.showSnackBar(SnackBar(content: Text(value)));
  }

  void _handleSubmitted() async {
    final FormState form = _formKey.currentState;
    if (!form.validate()) {
      showInSnackBar('Please fix the errors in red before submitting.');
    } else {
      form.save();

      SharedPreferences prefs = await SharedPreferences.getInstance();
      prefs.setString("ip", _ip);
      Navigator.pushNamedAndRemoveUntil(
          context, '/', ModalRoute.withName('/'));
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        key: _scaffoldKey,
        appBar: AppBar(
          title: Text('Login'),
        ),
        body: SafeArea(
          top: false,
          bottom: false,
          child: Form(
            key: _formKey,
            child: SingleChildScrollView(
              padding: const EdgeInsets.symmetric(horizontal: 16.0),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: <Widget>[
                  TextFormField(
                    key: Key("_mobile"),
                    decoration: InputDecoration(labelText: "IP-adress"),
                    keyboardType: TextInputType.numberWithOptions(decimal: true),
                    initialValue: _ip,
                    onSaved: (String value) {
                      _ip = value;
                    },
                    validator: (value) {
                      if (value.isEmpty) {
                        return 'IP is required';
                      }
                      return null;
                    },
                  ),

                  const SizedBox(height: 10.0),
                  ButtonBar(
                    children: <Widget>[
                      RaisedButton.icon(
                          onPressed: _handleSubmitted,
                          icon: Icon(Icons.arrow_forward),
                          label: Text('Підключитись')),
                    ],
                  )
                ],
              ),
            ),
          ),
        ));
  }
}