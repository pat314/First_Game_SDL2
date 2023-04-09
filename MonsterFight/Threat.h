
#ifndef THREAT_H
#include "CommonFunc.h"
#include "BaseObject.h"
#include "Bullet.h"


class Threat : public BaseObject
{
public:
    Threat();
    ~Threat();
    //hàm set - get cho val
    void setThreat_val (const float& xVal, const float& yVal) {x_val_threat = xVal; y_val_threat = yVal;};
    double getThreat_x_val () const {return x_val_threat;};
    double getThreat_y_val () const {return y_val_threat;};
    //hàm set - get cho position
    void setThreat_position (const float& x, const float& y) {x_threat = x; y_threat = y;};
    double getThreat_x_pos () const {return x_threat;};
    double getThreat_y_pos () const {return x_threat;};
    //Hàm set - get Map
    void getThreat_MapXY (const int& mpX, const int& mpY) {map_X_threat = mpX; map_Y_threat = mpY;};
    //Hàm cắt sprite thành các frame
    void setThreat_frame ();
    //Hàm kiểm tra việc load ảnh đồng thời khởi tạo các tham số để cắt ảnh thành các frame
    bool loadThreat_img (std:: string path, SDL_Renderer* screen);
    //Hàm để show các frame đã cắt để có ảnh động của threat
    void showThreat_sprite (SDL_Renderer* des);
    //Hàm get kích thước của frame
    int getThreat_w_frame() const {return w_frame_threat;};
    int getThreat_h_frame() const {return h_frame_threat;};
    SDL_Rect GetRectFrame();
    //Hàm xử lý chuyển động của threat trên màn hình
    void doThreat (Map& gMap, SDL_Renderer* screen);
    //hàm xử lý threat va chạm với map
    void checkThreat_ground (Map& mapData);

    void setThreat_typeMove (const int& _typeMove) {typeMove = _typeMove;};
    void setThreat_animationInterval (const int& a, const int& b) {animationThreat_a_interval = a; animationThreat_b_interval = b;};
    void setThreat_inputLeft (const int& ipLeft) {inputType_threat.left = ipLeft;};
    int getThreat_inputLeft () const {return inputType_threat.left;};
    //hàm xử lý di chuyển
    void ImplementMoveType_threat  (SDL_Renderer* screen);
    //hàm khởi tạo lại threat sau khi threat rơi xuống vực
    void initThreat ();
    //hàm set-get cho mảng đạn bắn
    std::vector <Bullet*> getThreat_bulletList () const {return bulletThreat_list;};
    void setThreat_bulletList (const std:: vector<Bullet*>& bullet_list) {bulletThreat_list = bullet_list;};
    //Hàm tạo đạn
    void initThreat_bullet (Bullet* bullet, SDL_Renderer* screen);
    void MakeBullet (SDL_Renderer* screen, const int& x_limit, const int& y_limit, Map& mapData);
    //Hàm bỏ ảnh viên đạn thứ index sau khi bắn trúng player
    void ClearThreatBullet (const int& index);

    void ResetBullet(Bullet* p_bullet);


private:
    //Chỉ số của frame hiện đang xét trong hàm
    int frame_threat;
    //Kích thước của frame
    int w_frame_threat;
    int h_frame_threat;
    //Mảng lưu các frame (sau khi cắt từ sprite)
    SDL_Rect frame_clip_threat [THREAT_FRAME_NUM];
    //Biến lưu trạng thái có trên mặt đất không?
    bool isOnGround_threat;
    //Biến lưu thời gian chờ hồi sinh của threat
    int revive_time_threat;
    //Biến lưu vị trí threat
    double x_threat;
    double y_threat;
    //Biến đặc trưng cho việc di chuyển của threat trong 1s (vận tốc)
    double x_val_threat;
    double y_val_threat;
    //Biến lưu giới hạn của nhân vật threat đối với bản đồ (vd nhưu rơi xuống vực)
    int map_X_threat;
    int map_Y_threat;

    int typeMove;
    //Biến giới hạn khu vực di chuyển của threat động
    int animationThreat_a_interval;
    int animationThreat_b_interval;
    Control inputType_threat;
    bool xCollision;
    //Mảng quản lý đạn bắn cho threat
    std::vector<Bullet*> bulletThreat_list;
    //Chiều đạn bắn của threat động
    int ThreatbulletDir;
};

#endif // THREAT_H
