import 'dart:math';

import 'package:flutter/material.dart';

class BoardCanvas extends CustomPainter {
  @override
  void paint(Canvas canvas, Size size) {
    var paint = Paint()
      ..style = PaintingStyle.stroke
      ..color = Colors.blueGrey[400]
      ..isAntiAlias = true
      ..strokeWidth = 1;

    double space = size.width / 10;

    Path path = Path();
    for (double i = 0; i < 11; ++i) {
      path.moveTo(space * i, 0);
      path.lineTo(space * i, space * 10);
    }
    for (double i = 0; i < 11; ++i) {
      path.moveTo(0, space * i);
      path.lineTo(space * 10, space * i);
    }

    canvas.drawPath(path, paint);
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) => true;
}

class MatrixCanvas extends StatefulWidget {
  @override
  _MatrixCanvasState createState() => _MatrixCanvasState();
}

class _MatrixCanvasState extends State<MatrixCanvas> {


  @override
  Widget build(BuildContext context) {
    final _size = MediaQuery.of(context).size;
    final size = min(_size.height, _size.width) - 40;
    final sizeCell = size / 10;

    return Container(
      width: size,
      height: size,
      child: CustomPaint(
        painter: BoardCanvas(),
      ),
    );
  }
}
