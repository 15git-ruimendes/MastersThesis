class PI_Controller
{
public:
    float Kp;
    float Ki;
    float Upper_Limit;
    float Lower_Limit;
    float Delta;
    // Insert Kp, Ki, Upper Limit, Lower Limit and either frequency or time step
    // If using time_step make sure it's less than 0
    PI_Controller(float Kp_req, float Ki_req, float Upper_Limit_req, float Lower_Limit_req, float Delta_Freq_req = 0)
    {
        Kp = Kp_req;
        Ki = Ki_req;
        Upper_Limit = Upper_Limit_req;
        Lower_Limit = Lower_Limit_req;
        if (Delta_Freq_req < 1)
        {
            Delta = Delta_Freq_req;
        }
        else
        {
            Delta = 1 / Delta_Freq_req;
        }
    };
    float Update_Gains(float Kp_update, float Ki_update)
    {
        Kp = Kp_update;
        Ki = Ki_update;
    }
    float Calculate_OutPut(float Error)
    {
        float res = 0;
        //
        // Proportional and Integral Calc.
        //

        Error_acm += Error * Delta;
        //
        // Anti-Windup (Not needed)
        //

        //
        // Result Calculation
        //
        res = Ki * Error_acm + Kp * Error;
        //
        // Limiter
        //
        if (res > Upper_Limit)
            return Upper_Limit;
        else if (res < Lower_Limit)
            return Lower_Limit;
        else
            return res;
    };

private:
    float Error_acm = 0;
};
