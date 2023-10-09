
// #include "../src/Android/android.h"
// #include "../src/Autosar/autosar.h"

// int test1(){
//     IAndroid* agent = new Android;
//     IAutosar* autosar = new Autosar;

//     agent->init(autosar);
//     message msg;
//     while (1)
//     {
//         //从标准输入读取数据指定fid
//         std::cout << "please input fid:";
//         std::cin >> msg.fid;
//         agent->Send(&msg);
//         std::cout << "msg::output:" << msg.output << std::endl;
//     }
    
//     return 0;
// }

// int main(){
//     test1();
//     return 0;
// }

// // #include "project/tmp.hpp"

// // #include <gtest/gtest.h>

// // TEST(TmpAddTest, CheckValues)
// // {
// //   ASSERT_EQ(tmp::add(1, 2), 3);
// //   EXPECT_TRUE(true);
// // }

// // TEST(TmpSubTest, CheckValues)
// // {
// //   ASSERT_EQ(tmp::sub(1, 2), -1);
// //   EXPECT_TRUE(true);
// // }

// // TEST(TmpMulTest, CheckValues)
// // {
// //   ASSERT_EQ(tmp::mul(1, 2), 2);
// //   EXPECT_TRUE(true);
// // }

// // TEST(TmpDivTest, CheckValues)
// // {
// //   ASSERT_EQ(tmp::div(1, 2), 0);
// //   EXPECT_TRUE(true);
// // }

// // int main(int argc, char **argv)
// // {
// //   ::testing::InitGoogleTest(&argc, argv);
// //   return RUN_ALL_TESTS();
// // }
