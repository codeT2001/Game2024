#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <functional>
#include <QWidget>
#include <QRegularExpression>
#include <QPoint>
extern QString gate_url_prefix;

extern std::function<void(QWidget*)> repolish;

extern std::function<QString(QString)> xorString;

enum MSG_IDS{
    MSG_HW = 2047,              //测试
    MSG_GET_CODE = 1001,        //获取验证码
    MSG_LOGIN = 1002,           //请求登录
    MSG_REGISTER = 1003,        //请求注册
    MSG_RESET = 1004,           //请求重置密码
    MSG_LOGIN_REP = 1005,       //登录回复
    MSG_FIND_SUCCESS = 1006,    //寻找用户成功
    MSG_FIND_FAILED = 1007,     //寻找用户失败
    MSG_LOGIN_SUCCESS = 1008,   //登陆成功
    MSG_MATCH = 1009,           //匹配游戏
    MSG_CANCEL_MATCH = 1010,    //取消匹配
    MSG_MACTH_SUCCESS = 1011,   //匹配成功
    MSG_CHAT_GAME = 1012,       //游戏中聊天
    MSG_CHAT_FRIENDS = 1013,    //好友聊天
    MSG_MOVE = 1014,            //棋子移动
    MSG_GAMEOVER = 1015,        //游戏结束
    MSG_REGISTER_REP = 1016,    //注册回包
    MSG_RESET_REP = 1017,       //重置密码回包
    MSG_GIVEUP = 1018,          //投降
    MSG_DEL_FRIEND = 1019,      //删除朋友
    MSG_WATHCH = 1020,          //观战
    MSG_INVITE = 1021,          //邀请
    MSG_ADD_PLAYER = 1022,      //添加好友
    MSG_ADD_APPLY = 1023,       //界面列表添加申请消息
    MSG_ACCEPT_ADD = 1024,      //同意添加
    MSG_LOAD_APPLY = 1025,      //从服务端下载数据
    MSG_LODA_FRIENDS = 1026,    //加载朋友数据
    MSG_CHAT_INFO = 1027,       //好友聊天
    //MSG_STATE_CHANGE = 1028,
};




enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VERIFY_ERR = 5,
    TIP_USER_ERR = 6,
    TIP_REGISTER_ERR = 7,
    TIP_LOGIN_FAILED = 8,
};

enum ClickLbState{
    Normal = 0,
    Selected = 1
};

enum COLOR{
    BLACK = 555,
    WHITE = 666,
};

enum ListItemType{
    CONTACT_USER_ITEM,      //联系人用户
    SEARCH_USER_ITEM,       //搜索到的用户
    ADD_USER_ITEM,          //提示添加用户
    INVALID_ITEM,           //不可点击
};
//enum STATES{
//    ONLINE = 0,             //在线
//    PLAYING = 1,            //游戏中
//    WATCHING = 2,           //观战中
//    MATCHING = 3,           //匹配中
//    OFFLINE = 4,            //离线
//};

//const std::vector<QString>  stateString ={"在线","游戏中", "观战中","匹配中","离线"};
typedef struct Item{
    QPoint pt;
    COLOR color;
    bool operator ==(const Item& item) const
    {
        return pt == item.pt && color == item.color;
    }
}Item;



//const std::vector<QString> levels = {
//    "青铜",
//    "白银",
//    "黄金",
//    "白金",
//    "钻石"
//};

#endif // GLOBAL_H
