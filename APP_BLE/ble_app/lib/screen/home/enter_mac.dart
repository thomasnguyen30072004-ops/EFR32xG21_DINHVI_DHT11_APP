import 'package:ble_app/screen/home/home_screen.dart';
import 'package:flutter/material.dart';

class EnterMac extends StatefulWidget {
  const EnterMac({super.key});

  @override
  State<EnterMac> createState() => _EnterMacState();
}

class _EnterMacState extends State<EnterMac> {

  final one_controller = TextEditingController();
  final two_controller = TextEditingController();
  final three_controller = TextEditingController();


  @override
  Widget build(BuildContext context) {
    return Scaffold(
        body: ListView(
          padding: EdgeInsets.all(20),
          children: [
            SizedBox(height: 50,),
            Text('MAC BRD4180B Board',textAlign: TextAlign.center,style: TextStyle(fontSize: 30,fontWeight: FontWeight.bold),),
            SizedBox(height: 30,),
            TextField(
              controller: one_controller,
              decoration: InputDecoration(

                hintText: 'Nhập địa chỉ Mac trạm 1',
                border: OutlineInputBorder()
              ),
            ),
            SizedBox(height: 20,),
            TextField(
              controller: two_controller,
              decoration: InputDecoration(
                  hintText: 'Nhập địa chỉ Mac trạm 2',
                  border: OutlineInputBorder()
              ),
            ),
            SizedBox(height: 20,),
            TextField(
              controller: three_controller,
              decoration: InputDecoration(
                  hintText: 'Nhập địa chỉ Mac trạm 3',
                  border: OutlineInputBorder()
              ),
            ),
            SizedBox(height: 20,),
            ElevatedButton(
              style: ElevatedButton.styleFrom(
                backgroundColor: Colors.red,
                foregroundColor: Colors.white
              ),
                onPressed: (){
                  Navigator.pushReplacement(context, MaterialPageRoute(builder: (context) => IndoorNavPage(
                      MAC_ANCHOR_1: one_controller.text.trim().toUpperCase(),
                      MAC_ANCHOR_2: two_controller.text.trim().toUpperCase(),
                      MAC_ANCHOR_3: three_controller.text.trim().toUpperCase()
                  ),
                  )
                  );
                },
                child: Text('Next')
            )
          ],
        ),
    );
  }
}
