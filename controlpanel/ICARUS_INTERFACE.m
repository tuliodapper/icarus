function varargout = ICARUS_INTERFACE(varargin)
% ICARUS_INTERFACE MATLAB code for ICARUS_INTERFACE.fig
%      ICARUS_INTERFACE, by itself, creates a new ICARUS_INTERFACE or raises the existing
%      singleton*.
%
%      H = ICARUS_INTERFACE returns the handle to a new ICARUS_INTERFACE or the handle to
%      the existing singleton*.
%
%      ICARUS_INTERFACE('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in ICARUS_INTERFACE.M with the given input arguments.
%
%      ICARUS_INTERFACE('Property','Value',...) creates a new ICARUS_INTERFACE or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before ICARUS_INTERFACE_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to ICARUS_INTERFACE_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help ICARUS_INTERFACE

% Last Modified by GUIDE v2.5 05-Apr-2015 23:56:43

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @ICARUS_INTERFACE_OpeningFcn, ...
                   'gui_OutputFcn',  @ICARUS_INTERFACE_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT

function SendParameter(bt, chr, value)
    fprintf(bt, '<%c%d>', [chr, value]);

function FillTable(obj)
    load('config.mat');
    set(obj, 'RowName', array_name, 'Data', array_data);

function DisplaySerial(handles)
    MAX = 300;
    velocity = zeros(MAX);
    control  = zeros(MAX);
    i = 2;

    set(handles.txtSerial,'String',''); 

    drawnow;

    while(strcmp(get(handles.btnDisplayVelocityStart,'Enable'),'off'))

        if (strcmp(get(handles.txtSerial,'Visible'), 'on'))
            s_string = char(get(handles.txtSerial,'String'), fgets(handles.bt));

            size = length(s_string);

            if (size > 200)
                s_string = s_string(size-200:size,:);
            end

            set(handles.txtSerial,'String', s_string);
        else
            parameter = fscanf(handles.bt,'%*c%c%4d%*c');
            if (parameter(2) == 0)
                if (i>(MAX/2))
                    velocity(i) = parameter(2);
                    control(i) = parameter(4);

                    time = 0:(i-1);

                    axes(handles.axesVelocity);
                    plot(time,velocity(1:i));
                    title('Velocity');

                    axes(handles.axesControl);
                    plot(time,control(1:i));
                    title('Control');
                end
                i = 2;
            else
                velocity(i) = parameter(2);
                control(i) = parameter(4);
                i=i+1;
            end
        end
        drawnow;
    end
    
function drawFigure(pos, color, value)        
    rectangle('Position',pos,'Curvature',[1 1], 'FaceColor', color)
    if (~isnan(value))
        text(pos(1)+0.1,pos(2)-0.06,num2str(value));
    end

function ShowSensor(handles)
    
    GREEN = [0 1 0];
    WHITE = [1 1 1];
   
    while(strcmp(get(handles.btnDisplaySensorStart,'Enable'),'off'))

%         h1 = figure(1);
%         clf;
%         hold on;
%         axis equal
%         axis off
%         drawFigure([2 4 2 2], 'w', NaN);

        sensor = fscanf(handles.bt,'%*c%c%4d%*c');
        
        if (sensor(2) > 100) color = GREEN; else color = WHITE; end
        set(handles.pLedFL, 'BackgroundColor', color);
        set(handles.txtLedFL, 'String', num2str(sensor(2)));
%         drawFigure([2.2 5.2 0.2 0.2], color, sensor(2));

        if (sensor(4) > 100) color = GREEN; else color = WHITE; end
        set(handles.pLedFR, 'BackgroundColor', color);
        set(handles.txtLedFR, 'String', num2str(sensor(4)));
%         drawFigure([3.6 5.2 0.2 0.2], color, sensor(4));
        
        if (sensor(6) > 100) color = GREEN; else color = WHITE; end
        set(handles.pLedDL, 'BackgroundColor', color);
        set(handles.txtLedDL, 'String', num2str(sensor(6)));
%         drawFigure([2.6 5.35 0.2 0.2], color, sensor(6));
        if (sensor(8) > 100) color = GREEN; else color = WHITE; end
        set(handles.pLedDR, 'BackgroundColor', color);
        set(handles.txtLedDR, 'String', num2str(sensor(8)));
%         drawFigure([3.2 5.35 0.2 0.2], color, sensor(8));
        
        if (sensor(10) > 100) color = GREEN'; else color = WHITE; end
        set(handles.pLedL, 'BackgroundColor', color);
        set(handles.txtLedL, 'String', num2str(sensor(10)));
%         drawFigure([2.7 5.7 0.2 0.2], color, sensor(10));
        if (sensor(12) > 100) color = GREEN; else color = WHITE; end
        set(handles.pLedR, 'BackgroundColor', color);
        set(handles.txtLedR, 'String', num2str(sensor(12)));
%         drawFigure([3.1 5.7 0.2 0.2], color, sensor(12));
        drawnow;
    end

function ShowMapping(handles)

    sizeX = 8;
    sizeY = 6;

    NORTH = 1;
    EAST = 2;
    SOUTH = 3;
    WEST = 4;
   
    % Array contains wall info north south east west of each cell
    maze_walls = zeros(sizeX, sizeY, 4);
    % Variable initialisation
    current_x = 1;
    current_y = sizeY;
    
    while(strcmp(get(handles.btnMoveStart,'Enable'),'off'))

        %% Plot maze
        draw_maze(sizeX, sizeY, current_x, current_y, maze_walls);
        
        SendParameter(handles.bt,'O',0);
        [current_x, current_y, walls] = receive_info(handles.bt);
        
        if (current_x == 99) && (current_y == 99)  
            % Array contains wall info north south east west of each cell
            maze_walls = zeros(sizeX, sizeY, 4);
            % Variable initialisation
            current_x = 1;
            current_y = sizeY;
        else
            current_y = sizeY - current_y;
            current_x = current_x + 1;
            maze_walls(current_x, current_y, WEST) = bitget(walls, 1);
            maze_walls(current_x, current_y, NORTH) = bitget(walls, 2);
            maze_walls(current_x, current_y, EAST) = bitget(walls, 3);
            maze_walls(current_x, current_y, SOUTH) = bitget(walls, 4);
        end
    end
    close(h1);

% --- Executes just before ICARUS_INTERFACE is made visible.
function ICARUS_INTERFACE_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to ICARUS_INTERFACE (see VARARGIN)

% Choose default command line output for ICARUS_INTERFACE
handles.output = hObject;
% Update handles structure
guidata(hObject, handles);

% UIWAIT makes ICARUS_INTERFACE wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = ICARUS_INTERFACE_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of pushbutton2

% --- Executes during object creation, after setting all properties.
function tbParameters_CreateFcn(hObject, eventdata, handles)
% hObject    handle to tbParameters (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
FillTable(hObject);

% --- Executes on button press in btnSaveAll.
function btnSaveAll_Callback(hObject, eventdata, handles)
% hObject    handle to btnSaveAll (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if (~isfield(handles,'bt'))
    msgbox('Icarus Mobile not connected!');
else
    tableData = get(handles.tbParameters, 'Data');
    h = waitbar(0, 'Sending data...');
    total_rows = length(tableData(:,1));
    for row = 1: total_rows
        waitbar(row/total_rows, h, 'Sending data...');
        SendParameter(handles.bt, char(tableData(row,1)), cell2mat(tableData(row,2)));
        pause(0.1);
    end
    close(h);
end


% --- Executes on button press in btnConnect.
function btnConnect_Callback(hObject, eventdata, handles)
% hObject    handle to btnConnect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Hint: get(hObject,'Value') returns toggle state of btnConnect
if (~isfield(handles,'bt'))
    handles.bt = Bluetooth('Icarus',1);
end
guidata(hObject, handles) 
button_state = get(hObject,'Value');
if button_state == get(hObject,'Max')
	fopen(handles.bt);
    set(hObject, 'String', 'DISCONNECT');
elseif button_state == get(hObject,'Min')
	fclose(handles.bt);
    handles = rmfield(handles, 'bt');
    guidata(hObject, handles) 
    set(hObject, 'String', 'CONNECT');
end

    
% --- Executes when entered data in editable cell(s) in tbParameters.
function tbParameters_CellEditCallback(hObject, eventdata, handles)
% hObject    handle to tbParameters (see GCBO)
% eventdata  structure with the following fields (see UITABLE)
%	Indices: row and column indices of the cell(s) edited
%	PreviousData: previous data for the cell(s) edited
%	EditData: string(s) entered by the user
%	NewData: EditData or its converted form set on the Data property. Empty if Data was not changed
%	Error: error string when failed to convert EditData to appropriate value for Data
% handles    structure with handles and user data (see GUIDATA)
if (~isfield(handles,'bt'))
    msgbox('Icarus Mobile not connected!');
else
    Indice = eventdata.Indices;
    row = Indice(1);
    col = Indice(2);
    if (col == 2)
        tableData = get(hObject, 'Data');
        SendParameter(handles.bt, char(tableData(row,1)), cell2mat(tableData(row,2)));
    end
end


% --- Executes on button press in btnSave.
function btnSave_Callback(hObject, eventdata, handles)
% hObject    handle to btnSave (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

array_data = get(handles.tbParameters, 'Data');
array_name = get(handles.tbParameters, 'RowName');

assignin('base','array_data',array_data)
assignin('base','array_name',array_name)

save('config.mat', 'array_data', 'array_name');

% --- Executes on button press in btnReset.
function btnReset_Callback(hObject, eventdata, handles)
% hObject    handle to btnReset (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
FillTable(handles.tbParameters);


% --- Executes when user attempts to close figure1.
function figure1_CloseRequestFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if (isfield(handles,'bt'))
	fclose(handles.bt);
    handles = rmfield(handles, 'bt');
end
% Hint: delete(hObject) closes the figure
delete(hObject);

% --- Executes during object creation, after setting all properties.
function txtSerial_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtSerial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

jhEdit = findjobj(hObject);
jhEdit.anchorToBottom;


% --- Executes on button press in chkMoveMapping.
function chkMoveMapping_Callback(hObject, eventdata, handles)
% hObject    handle to chkMoveMapping (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of chkMoveMapping
if (~isfield(handles,'bt'))
    msgbox('Icarus Mobile not connected!');
    set(hObject,'Value', get(hObject,'Min'));
else
    button_state = get(hObject,'Value');
    if button_state == get(hObject,'Max')
        SendParameter(handles.bt,'m',1);
    elseif button_state == get(hObject,'Min')
        SendParameter(handles.bt,'m',0);
    end
end
    

% --- Executes on button press in btnMoveStart.
function btnMoveStart_Callback(hObject, eventdata, handles)
% hObject    handle to btnMoveStart (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if (~isfield(handles,'bt'))
    msgbox('Icarus Mobile not connected!');
else
    SendParameter(handles.bt,'M',1);
    
    set(hObject, 'Enable', 'off');
    
    set(handles.btnMoveStop, 'Enable', 'on');
    
    set(handles.btnMovePause, 'Enable', 'on');
    
    set(handles.chkMoveMapping, 'Enable', 'off');
    
    drawnow;
    
    if (get(handles.chkMoveMapping,'Value') == get(hObject,'Max'))
        handles.VarShowMapping = 1;
        guidata(hObject, handles);
        ShowMapping(handles);
        handles = guidata(hObject);
        if (handles.VarShowMapping ~= 1)
            SendParameter(handles.bt,'M', handles.VarShowMapping);
        end
        handles = rmfield(handles, 'VarShowMapping');
        guidata(hObject, handles);
    end
end


% --- Executes on button press in btnMovePause.
function btnMovePause_Callback(hObject, eventdata, handles)
% hObject    handle to btnMovePause (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if (~isfield(handles,'bt'))
    msgbox('Icarus Mobile not connected!');
else
    SendParameter(handles.bt,'M',0);
    
    set(hObject, 'Enable', 'off');
    
    set(handles.btnMoveStart, 'Enable', 'on');
    
    set(handles.btnMoveStop, 'Enable', 'on');
    
    if (isfield(handles,'VarShowMapping'))
        handles.VarShowMapping = 0;
        guidata(hObject, handles);
    else
        SendParameter(handles.bt,'M', 0);
    end
end


% --- Executes on button press in btnMoveStop.
function btnMoveStop_Callback(hObject, eventdata, handles)
% hObject    handle to btnMoveStop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if (~isfield(handles,'bt'))
    msgbox('Icarus Mobile not connected!');
else
    set(hObject, 'Enable', 'off');
    
    set(handles.btnMoveStart, 'Enable', 'on');
    
    set(handles.btnMovePause, 'Enable', 'off');
    
    set(handles.chkMoveMapping, 'Enable', 'on');
    
    if (isfield(handles,'VarShowMapping'))
        handles.VarShowMapping = 2;
        guidata(hObject, handles);
    else
        SendParameter(handles.bt,'M', 2);
    end
 
end


% --- Executes on button press in btnDisplayVelocityStart.
function btnDisplayVelocityStart_Callback(hObject, eventdata, handles)
% hObject    handle to btnDisplayVelocityStart (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of btnDisplayVelocityStart
if (~isfield(handles,'bt'))
    msgbox('Icarus Mobile not connected!');
else
    set(hObject, 'Enable', 'off');
    set(handles.btnDisplaySensorStart, 'Enable', 'off');
    
    set(handles.btnDisplayVelocityStop, 'Enable', 'on');
    
    SendParameter(handles.bt,'U',1);
    DisplaySerial(handles);
    SendParameter(handles.bt,'U',0);
end

% --- Executes on button press in btnDisplaySensorStart.
function btnDisplaySensorStart_Callback(hObject, eventdata, handles)
% hObject    handle to btnDisplaySensorStart (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of btnDisplaySensorStart
if (~isfield(handles,'bt'))
    msgbox('Icarus Mobile not connected!');
else
    set(hObject, 'Enable', 'off');
    set(handles.btnDisplayVelocityStart, 'Enable', 'off');
    
    set(handles.btnDisplaySensorStop, 'Enable', 'on');
    
    SendParameter(handles.bt,'s',1);
    ShowSensor(handles);
    SendParameter(handles.bt,'s',0);
    
end


% --- Executes on button press in btnDisplayVelocityStop.
function btnDisplayVelocityStop_Callback(hObject, eventdata, handles)
% hObject    handle to btnDisplayVelocityStop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of btnDisplayVelocityStop
if (~isfield(handles,'bt'))
    msgbox('Icarus Mobile not connected!');
else
    set(hObject, 'Enable', 'off');
    
    set(handles.btnDisplayVelocityStart, 'Enable', 'on');
    set(handles.btnDisplaySensorStart, 'Enable', 'on');
end

% --- Executes on button press in btnDisplaySensorStop.
function btnDisplaySensorStop_Callback(hObject, eventdata, handles)
% hObject    handle to btnDisplaySensorStop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of btnDisplaySensorStop
if (~isfield(handles,'bt'))
    msgbox('Icarus Mobile not connected!');
else
    set(hObject, 'Enable', 'off');
    
    set(handles.btnDisplayVelocityStart, 'Enable', 'on');
    set(handles.btnDisplaySensorStart, 'Enable', 'on');
end


% --- Executes on button press in pushbutton11.
function pushbutton11_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton11 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes during object creation, after setting all properties.
function figure1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
movegui(hObject, 'center');


% --- Executes during object creation, after setting all properties.
function btnReset_CreateFcn(hObject, eventdata, handles)
% hObject    handle to btnReset (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes on button press in rText.
function rText_Callback(hObject, eventdata, handles)
% hObject    handle to rText (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
    set(handles.rGraph, 'Value', get(handles.rGraph,'Min'));
    set(handles.axesVelocity, 'Visible', 'off');
    set(handles.axesControl, 'Visible', 'off');
    set(handles.txtSerial, 'Visible', 'on');
% Hint: get(hObject,'Value') returns toggle state of rText


% --- Executes on button press in rGraph.
function rGraph_Callback(hObject, eventdata, handles)
% hObject    handle to rGraph (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
    set(handles.rText, 'Value', get(handles.rGraph,'Min'));
    set(handles.txtSerial, 'Visible', 'off');
    set(handles.axesVelocity, 'Visible', 'on');
    set(handles.axesControl, 'Visible', 'on');

% Hint: get(hObject,'Value') returns toggle state of rGraph


% --- Executes on button press in radiobutton6.
function radiobutton6_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton6


% --- Executes on button press in radiobutton7.
function radiobutton7_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton7
