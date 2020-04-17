#ifndef _THREADLOOP_H_
#define _THREADLOOP_H_
#include <pthread.h>

#include "common/fileIO/include/jsonparse.h"
#include "common/fileIO/recorder/include/datarecorder.h"
#include "common/logging/include/easylogging++.h"
#include "common/timer/include/timecounter.h"
#include "modules/controller/include/controller.h"
#include "modules/controller/include/trajectorytracking.h"
#include "modules/envirment_simu/current/include/current.h"
#include "modules/envirment_simu/wave/wave.h"
#include "modules/envirment_simu/wind/include/windcompensation.h"
#include "modules/estimator/include/estimator.h"
#include "modules/simulator/include/simulator.h"

namespace ASV {

const std::string parameter_json_path = "./../../properties/property.json";
constexpr int num_thruster = 2;
constexpr int dim_controlspace = 3;
constexpr localization::USEKALMAN indicator_kalman =
    localization::USEKALMAN::KALMANOFF;
constexpr control::ACTUATION indicator_actuation =
    control::ACTUATION::UNDERACTUATED;
constexpr int max_num_targets = 20;

constexpr common::TESTMODE testmode = common::TESTMODE::SIMULATION_DP;
// constexpr common::TESTMODE testmode = common::TESTMODE::SIMULATION_LOS;

class threadloop {
 public:
  threadloop()
      : _currentcompensation(currentloadRTdata),
        _mywave(5, 9),
        _windcompensation(windloadRTdata),
        _jsonparse(parameter_json_path) {
    std::cout << "后造函数" << std::endl;
  };
  ~threadloop(){};

  void mainloop() {
    std::thread controller_thread(&threadloop::controllerloop, this);
    std::thread estimator_thread(&threadloop::estimatorloop, this);
    std::thread sql_thread(&threadloop::sqlloop, this);
    std::thread seaload_thread(&threadloop::sealoadloop, this);

    estimator_thread.join();
    controller_thread.join();
    sql_thread.join();
    seaload_thread.join();
  }

 private:
  /********************* Real time Data  *********************/
  // planning::RoutePlannerRTdata RoutePlanner_RTdata{
  //     common::STATETOGGLE::IDLE,  // state_toggle
  //     0,                          // setpoints_X
  //     0,                          // setpoints_Y;
  //     0,                          // setpoints_heading;
  //     0,                          // setpoints_longitude;
  //     0,                          // setpoints_latitude;
  //     "OFF",                      // UTM zone
  //     0,                          // speed
  //     0,                          // los_capture_radius
  //     Eigen::VectorXd::Zero(2),   // Waypoint_X
  //     Eigen::VectorXd::Zero(2),   // Waypoint_Y
  //     Eigen::VectorXd::Zero(2),   // Waypoint_longitude
  //     Eigen::VectorXd::Zero(2)    // Waypoint_latitude
  // };
  // real time data of controller
  control::controllerRTdata<num_thruster, dim_controlspace> controller_RTdata{
      common::STATETOGGLE::IDLE,                           // state_toggle
      Eigen::Matrix<double, dim_controlspace, 1>::Zero(),  // tau
      Eigen::Matrix<double, dim_controlspace, 1>::Zero(),  // BalphaU
      Eigen::Matrix<double, num_thruster, 1>::Zero(),      // command_u
      Eigen::Matrix<int, num_thruster, 1>::Zero(),         // command_rotation
      Eigen::Matrix<double, num_thruster, 1>::Zero(),      // command_alpha
      Eigen::Matrix<int, num_thruster, 1>::Zero(),         // command_alpha_deg
      Eigen::Matrix<double, num_thruster, 1>::Zero(),      // feedback_u
      Eigen::Matrix<int, num_thruster, 1>::Zero(),         // feedback_rotation
      Eigen::Matrix<double, num_thruster, 1>::Zero(),      // feedback_alpha
      Eigen::Matrix<int, num_thruster, 1>::Zero()          // feedback_alpha_deg
  };

  // real time data of tracker
  control::trackerRTdata tracker_RTdata{
      control::TRACKERMODE::STARTED,  // trackermode
      Eigen::Vector3d::Zero(),        // setpoint
      Eigen::Vector3d::Zero()         // v_setpoint
  };

  // realtime parameters of the estimators
  localization::estimatorRTdata estimator_RTdata{
      common::STATETOGGLE::IDLE,            // state_toggle
      Eigen::Matrix3d::Identity(),          // CTB2G
      Eigen::Matrix3d::Identity(),          // CTG2B
      Eigen::Matrix<double, 6, 1>::Zero(),  // Measurement
      Eigen::Matrix<double, 6, 1>::Zero(),  // Measurement_6dof
      Eigen::Matrix<double, 6, 1>::Zero(),  // Marine_state
      Eigen::Matrix<double, 5, 1>::Zero(),  // radar_state
      Eigen::Matrix<double, 6, 1>::Zero(),  // State
      Eigen::Vector3d::Zero(),              // p_error
      Eigen::Vector3d::Zero(),              // v_error
      Eigen::Vector3d::Zero()               // BalphaU
  };

  Eigen::Vector3d windloadRTdata = Eigen::Vector3d::Zero();
  Eigen::Vector3d waveloadRTdata = Eigen::Vector3d::Zero();
  Eigen::Vector3d currentloadRTdata = Eigen::Vector3d::Zero();
  /********************* Modules  *********************/
  // json
  common::jsonparse<num_thruster, dim_controlspace> _jsonparse;
  ASV::localization::windcompensation _windcompensation;
  mywave _mywave;
  current _currentcompensation;

  void sealoadloop() {
    while (1) {
      //风力
      windloadRTdata = _windcompensation
                           .computewindload(2, 1.57, estimator_RTdata.State(3),
                                            estimator_RTdata.State(4),
                                            estimator_RTdata.State(2))
                           .getsealoadRTdata();  //风速、风向、船速u、v、船向
      std::cout << "????????????????????" << windloadRTdata(0) << "  "
                << windloadRTdata(1) << "  " << windloadRTdata(2) << std::endl;

      //浪
      _mywave.cal_wave_force(1.57, 100);  //遭遇频率和船长
      waveloadRTdata(0) = _mywave.get_wave_force().wave_fx;
      waveloadRTdata(1) = _mywave.get_wave_force().wave_fy;
      waveloadRTdata(2) = _mywave.get_wave_force().wave_fz;

      //流
      currentloadRTdata =
          _currentcompensation
              .computecurrentload(2, 1.57, estimator_RTdata.State(3),
                                  estimator_RTdata.State(4),
                                  estimator_RTdata.State(2))
              .getcurrentloadRTdata();  //流速、流向、船速u、v、船向
    }
  }

  void sqlloop() {
    std::cout << "db创建ing" << std::endl;
    // std::string sqlpath = _jsonparse.getsqlitepath();
    // std::string db_config_path = _jsonparse.getdbconfigpath();
    std::string sqlpath =
        "/home/skloe/Documents/毕业要紧/MultiVesselControl/examples/simu/utest/"
        "test/data/";
    std::string db_config_path =
        "/home/skloe/Documents/毕业要紧/MultiVesselControl/common/fileIO/"
        "recorder/config/dbconfig.json";
    // common::gps_db _gps_db(sqlpath, db_config_path);
    // common::stm32_db _stm32_db(sqlpath, db_config_path);
    // common::marineradar_db _marineradar_db(sqlpath, db_config_path);
    common::estimator_db _estimator_db(sqlpath, db_config_path);
    // std::cout << "db创建 1" << std::endl;
    // common::planner_db _planner_db(sqlpath, db_config_path);
    common::controller_db _controller_db(sqlpath, db_config_path);
    // std::cout << "db创建 2" << std::endl;
    // common::perception_db _perception_db(sqlpath, db_config_path);

    //_gps_db.create_table();
    //_stm32_db.create_table();
    //_marineradar_db.create_table();
    _estimator_db.create_table();
    //_planner_db.create_table();
    _controller_db.create_table();
    //_perception_db.create_table();
    std::cout << "db创建完成" << std::endl;
    while (1) {
      switch (testmode) {
        case common::TESTMODE::SIMULATION_DP: {
          {
            // simulation
            _estimator_db.update_measurement_table(
                common::est_measurement_db_data{
                    -1,                               // local_time
                    estimator_RTdata.Measurement(0),  // meas_x
                    estimator_RTdata.Measurement(1),  // meas_y
                    estimator_RTdata.Measurement(2),  // meas_theta
                    estimator_RTdata.Measurement(3),  // meas_u
                    estimator_RTdata.Measurement(4),  // meas_v
                    estimator_RTdata.Measurement(5)   // meas_r
                });
            _estimator_db.update_state_table(common::est_state_db_data{
                -1,                                // local_time
                estimator_RTdata.State(0),         // state_x
                estimator_RTdata.State(1),         // state_y
                estimator_RTdata.State(2),         // state_theta
                estimator_RTdata.State(3),         // state_u
                estimator_RTdata.State(4),         // state_v
                estimator_RTdata.State(5),         // state_r
                estimator_RTdata.Marine_state(3),  // curvature
                estimator_RTdata.Marine_state(4),  // speed
                estimator_RTdata.Marine_state(5)   // dspeed
            });
            _estimator_db.update_error_table(common::est_error_db_data{
                -1,                           // local_time
                estimator_RTdata.p_error(0),  // perror_x
                estimator_RTdata.p_error(1),  // perror_y
                estimator_RTdata.p_error(2),  // perror_mz
                estimator_RTdata.v_error(0),  // verror_x
                estimator_RTdata.v_error(1),  // verror_y
                estimator_RTdata.v_error(2)   // verror_mz
            });

            _controller_db.update_setpoint_table(
                common::control_setpoint_db_data{
                    -1,                            // local_time
                    tracker_RTdata.setpoint(0),    // set_x
                    tracker_RTdata.setpoint(1),    // set_y
                    tracker_RTdata.setpoint(2),    // set_theta
                    tracker_RTdata.v_setpoint(0),  // set_u
                    tracker_RTdata.v_setpoint(1),  // set_v
                    tracker_RTdata.v_setpoint(2)   // set_r
                });
            _controller_db.update_TA_table(common::control_TA_db_data{
                -1,                            // local_time
                controller_RTdata.tau(0),      // desired_Fx
                controller_RTdata.tau(1),      // desired_Fy
                controller_RTdata.tau(2),      // desired_Mz
                controller_RTdata.BalphaU(0),  // est_Fx
                controller_RTdata.BalphaU(1),  // est_Fy
                controller_RTdata.BalphaU(2),  // est_Mz
                std::vector<int>(controller_RTdata.command_alpha_deg.data(),
                                 controller_RTdata.command_alpha_deg.data() +
                                     num_thruster),  // alpha
                std::vector<int>(controller_RTdata.command_rotation.data(),
                                 controller_RTdata.command_rotation.data() +
                                     num_thruster)  // rpm
            });
            break;
          }
        }
        case common::TESTMODE::SIMULATION_LOS:

        default:
          break;
      }  // end switch
    }
  }  // sqlloop()

  void estimatorloop() {
    // initialization of estimator
    switch (testmode) {
      case common::TESTMODE::SIMULATION_DP: {
        // simulation
        localization::estimator<indicator_kalman,  // indicator_kalman
                                1,                 // nlp_x
                                1,                 // nlp_y
                                1,                 // nlp_z
                                1,                 // nlp_heading
                                1,                 // nlp_roll
                                1,                 // nlp_pitch
                                1,                 // nlp_u
                                1,                 // nlp_v
                                1                  // nlp_roti
                                >
            _estimator(estimator_RTdata, _jsonparse.getvessel(),
                       _jsonparse.getestimatordata());

        simulation::simulator _simulator(_jsonparse.getsimulatordata(),
                                         _jsonparse.getvessel());

        common::timecounter timer_estimator;
        long int outerloop_elapsed_time = 0;
        long int innerloop_elapsed_time = 0;
        long int sample_time =
            static_cast<long int>(1000 * _estimator.getsampletime());

        // State monitor toggle
        // StateMonitor::check_estimator();

        estimator_RTdata = _estimator.setvalue(3, 3, 0, 0, 0, 90, 0, 0, 0)
                               .getEstimatorRTData();
        _simulator.setX(estimator_RTdata.State);

        // real time calculation in estimator
        while (1) {
          outerloop_elapsed_time = timer_estimator.timeelapsed();

          auto x = _simulator
                       .simulator_onestep(tracker_RTdata.setpoint(2),
                                          controller_RTdata.tau)
                       .getX();
          // std::cout << "模拟器进入DP模式1" << std::endl;
          _estimator
              .updateestimatedforce(controller_RTdata.tau,
                                    Eigen::Vector3d::Zero())
              .estimatestate(x, tracker_RTdata.setpoint(2));
          // std::cout << "模拟器进入DP模式2" << std::endl;
          estimator_RTdata = _estimator
                                 .estimateerror(tracker_RTdata.setpoint,
                                                tracker_RTdata.v_setpoint)
                                 .getEstimatorRTData();
          // std::cout
          //     << "*********************************************************"
          //     << std::endl;

          // std::cout << "state: " << std::endl
          //           << estimator_RTdata.State << std::endl;
          // std::cout << "p_error: " << std::endl
          //           << estimator_RTdata.p_error << std::endl;
          // std::cout << "v_error: " << std::endl
          //           << estimator_RTdata.v_error << std::endl;
          // std::cout << "controller_RTdata.tau: " << std::endl
          //           << controller_RTdata.tau << std::endl;
          // std::cout << "controller_RTdata.BalphaU: " << std::endl
          //           << controller_RTdata.BalphaU << std::endl;
          // std::cout << "tracker_RTdata.setpoint: " << std::endl
          //           << tracker_RTdata.v_setpoint << std::endl;
          // std::cout << "tracker_RTdata.v_setpoint: " << std::endl
          //           << tracker_RTdata.setpoint << std::endl;

          innerloop_elapsed_time = timer_estimator.timeelapsed();
          std::this_thread::sleep_for(
              std::chrono::milliseconds(sample_time - innerloop_elapsed_time));

          if (outerloop_elapsed_time > 1.1 * sample_time)
            CLOG(INFO, "estimator") << "Too much time!";
        }

        break;
      }
      case common::TESTMODE::SIMULATION_LOS:

      default:
        break;
    }

  }  // estimatorloop()

  //################### path following, controller, TA ####################//
  void controllerloop() {
    control::controller<10, num_thruster, indicator_actuation, dim_controlspace>
        _controller(controller_RTdata, _jsonparse.getcontrollerdata(),
                    _jsonparse.getvessel(), _jsonparse.getpiddata(),
                    _jsonparse.getthrustallocationdata(),
                    _jsonparse.gettunneldata(), _jsonparse.getazimuthdata(),
                    _jsonparse.getmainrudderdata(),
                    _jsonparse.gettwinfixeddata());

    control::trajectorytracking _trajectorytracking(
        _jsonparse.getcontrollerdata(), tracker_RTdata);

    common::timecounter timer_controler;
    long int outerloop_elapsed_time = 0;
    long int innerloop_elapsed_time = 0;
    long int sample_time =
        static_cast<long int>(1000 * _controller.getsampletime());

    controller_RTdata =
        _controller.initializecontroller().getcontrollerRTdata();

    // _trajectorytracking.set_grid_points(
    //     RoutePlanner_RTdata.Waypoint_X, RoutePlanner_RTdata.Waypoint_Y,
    //     RoutePlanner_RTdata.speed, RoutePlanner_RTdata.los_capture_radius);

    while (1) {
      outerloop_elapsed_time = timer_controler.timeelapsed();

      switch (testmode) {
        case common::TESTMODE::SIMULATION_DP: {
          // std::cout << "控制进入DP模式" << std::endl;
          _controller.setcontrolmode(control::CONTROLMODE::DYNAMICPOSITION);
          tracker_RTdata.setpoint = Eigen::Vector3d::Zero();
          tracker_RTdata.v_setpoint = Eigen::Vector3d::Zero();

          _controller.set_thruster_feedback(
              controller_RTdata.command_rotation,
              controller_RTdata.command_alpha_deg);

          break;
        }
        case common::TESTMODE::SIMULATION_LOS: {
          _controller.setcontrolmode(control::CONTROLMODE::MANEUVERING);
          _controller.set_thruster_feedback(
              controller_RTdata.command_rotation,
              controller_RTdata.command_alpha_deg);
          // trajectory tracking
          _trajectorytracking.Grid_LOS(estimator_RTdata.State.head(2));
          tracker_RTdata = _trajectorytracking.gettrackerRTdata();

          break;
        }

        default:
          break;
      }  // end switch

      // controller
      controller_RTdata = _controller
                              .controlleronestep(Eigen::Vector3d::Zero(),
                                                 estimator_RTdata.p_error,
                                                 estimator_RTdata.v_error,
                                                 Eigen::Vector3d::Zero(),
                                                 tracker_RTdata.v_setpoint)
                              .getcontrollerRTdata();

      // std::cout << elapsed_time << std::endl;
      innerloop_elapsed_time = timer_controler.timeelapsed();
      std::this_thread::sleep_for(
          std::chrono::milliseconds(sample_time - innerloop_elapsed_time));

      if (outerloop_elapsed_time > 1.1 * sample_time)
        CLOG(INFO, "controller") << "Too much time!";
    }
  }  // controllerloop
};

}  // namespace ASV

#endif /* _THREADLOOP_H_ */