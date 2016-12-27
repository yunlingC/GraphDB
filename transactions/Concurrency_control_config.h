#ifndef _CONCURRENCY_CONTROL_CONFIG_H_
#define _CONCURRENCY_CONTROL_CONFIG_H_
/// TODO  to be delete
enum OperationType { NOOP, READ, UPDATE, INSERT, DELETE };

/// TODO need T_WAITING ?
enum TransStatusType {T_EXPANDING
                      , T_COMMIT
                      , T_ABORT
                      , T_ROLLBACK
                      , T_SHRINKING};

enum MutexType { T_ID
                , T_Property
//                , T_LastEdge
                , T_NextEdge
                , T_FirstVertex
                , T_SecondVertex
                , T_FirstNextEdge
                , T_FirstPrevEdge
                , T_SecondNextEdge
                , T_SecondPrevEdge
                , T_Label
              };

enum LockType { T_SH, T_EX };

enum LockRequestRetType { T_Abort,  T_Ignore,  T_Upgrade, T_Wait};


#endif /*_CONCURRENCY_CONTROL_CONFIG_H_*/
