class FiniteInterpretation:
    M = []
    def __init__(self, const, predicates, pr_v, functions, fun_v):
        self.M += const
        for i in range(len(predicates)):
            self.M.append(predicates[i])
            self.M.append(pr_v[i])
        for i in range(len(functions)):
            self.M.append(functions[i])
            self.M.append(fun_v[i])
    def func1(self, a, b):
        return int(a) + int(b)
    def func2(self, a, b):
        return int(a) - int(b)
    def pr1(self, a, b):
        return (int(a) + int(b)) % 2
    def count(self, func_name, arguments, variables):
        if func_name in ["→", "∧", "∨"]:
            if arguments[0] in variables:
                arguments[0] = variables[arguments[0]]
            if arguments[1] in variables:
                arguments[1] = variables[arguments[1]]
            if func_name == "→":
                if arguments[0] == 0:
                    return 1
                if arguments[1] >= 1:
                    return 1
                return 0
            if func_name == "∧":
                if arguments[0] == 0 or arguments[1] == 0:
                    return 0
                return 1
            if func_name == "∨":
                if arguments[0] >= 1 or arguments[1] >= 1:
                    return 1
                return 0
        if func_name == "¬":
            if arguments[0] in variables:
                arguments[0] = variables[arguments[0]]
            return not arguments[0]
        if func_name in self.M:
            if arguments[0] in variables:
                arguments[0] = variables[arguments[0]]
            if arguments[1] in variables:
                arguments[1] = variables[arguments[1]]
            if func_name == "+":
                return self.func1(arguments[0], arguments[1])
            if func_name == "-":
                return self.func2(arguments[0], arguments[1])
            if func_name == "p":
                return self.pr1(arguments[0], arguments[1])

class Signature:
    consts = []
    predicates = []
    predicates_valence = []
    functions = []
    functions_valence = []

    def __init__(self, c = [], p= [], pv = [], f= [], fv = []):
        self.consts = c
        self.predicates = p
        self.predicates_valence = pv
        self.functions = f
        self.functions_valence = fv




    def valence(self, arg):
        if arg in self.predicates:
            return self.predicates_valence[self.predicates.index(arg)]
        return self.functions_valence[self.functions.index(arg)]

def mx_in_dict(form):
    mx = -2
    for i in form:
        if i[0] >mx:
            mx = i[0]
    return mx

class Term:
    sign = Signature()
    form = {}
    ctr = -1
    variables = []
    def __init__(self, c = [], p= [], pv = [], f= [], fv = [], vars = []):
        self.sign = Signature(c, p, pv, f, fv)
        self.variables = vars

    def read(self, str, ctr = -1, pr = -1, huh = None):
        if str in self.sign.consts or str in self.variables:
            if (pr, ctr) not in self.form:
                self.form[(pr, ctr)] = []
            self.form[(pr, ctr)].append(str)
            return
        varts = []
        i = str.index('(') + 1
        j = str.index('(') + 1
        varts.append(str[:str.index('(')])
        while i < len(str):
            if str[i] == "(":
                cnt = 1
                while cnt > 0:
                    i += 1
                    if str[i] == "(":
                        cnt += 1
                    if str[i] == ")":
                        cnt -= 1
            if str[i] == ",":
                varts.append(str[j:i])
                j = i + 2
            i += 1
        varts.append(str[j:i - 1])
        if (pr, ctr) not in self.form:
            self.form[(pr, ctr)] = []
        self.form[(pr, ctr)].append(varts[0])
        if huh:
            mx = max(mx_in_dict(huh), mx_in_dict(self.form))
        else:
            mx = mx_in_dict(self.form)
        for i in range(1, len(varts)):
            self.read(varts[i], varts[0], mx+1)

    def show(self, ans = '', pr = -1, cnt = -1, ind = 0):
        ans += self.form[(pr, cnt)][ind]
        if self.form[(pr, cnt)][ind] not in self.variables and self.form[(pr, cnt)][ind] not in self.sign.consts:
            ans += "("
            t = 0
            for i in self.form[(pr+1, self.form[(pr, cnt)][ind])]:
                if i in self.sign.consts or i in self.variables:
                    ans += i + ", "
                else:
                    ans = self.show(ans, pr+1, self.form[(pr, cnt)][ind], t)
                    ans += ")"
                    ans += ", "
                t += 1
            ans = ans[:-2]
            ans += ")"
        return ans

def find_pls(form, i):
    for j in form:
        if j[0] == i:
            return j

class Formula:
    oper = ["→", "∧", "∨"]
    kv = ["∀", "∃"]
    trm = Term()
    form = {}

    def __init__(self, c=[], p=[], pv=[], f=[], fv=[], vars=[]):
        self.trm = Term(c, p, pv, f, fv, vars)

    def read(self, str, pr=-1, ctr=-1):
        if str in self.trm.variables:
            if (pr, ctr) not in self.form:
                self.form[(pr, ctr)] = []
            self.form[(pr, ctr)].append(str)
            return
        if ctr in self.trm.sign.predicates:
            self.trm.read(str, ctr, pr, self.form)
            for i in self.trm.form:
                if i in self.form:
                    self.trm.form[i] = self.form[i] + self.trm.form[i]
            self.form.update(self.trm.form)
            self.trm.form = {}
            return

        if str[0] == "¬":
            if (pr, ctr) not in self.form:
                self.form[(pr, ctr)] = []
            self.form[(pr, ctr)].append("¬")
            mx = mx_in_dict(self.form)
            self.read(str[2:-1], mx+1, "¬")
            return
        varts = []
        i = str.index('(') + 1
        j = str.index('(') + 1
        varts.append(str[:str.index('(')])
        while i < len(str):
            if str[i] == "(":
                cnt = 1
                while cnt > 0:
                    i += 1
                    if str[i] == "(":
                        cnt += 1
                    if str[i] == ")":
                        cnt -= 1
            if str[i] == ",":
                varts.append(str[j:i])
                j = i + 2
            i += 1
        varts.append(str[j:i - 1])
        if (pr, ctr) not in self.form:
            self.form[(pr, ctr)] = []
        self.form[(pr, ctr)].append(varts[0])
        mx = mx_in_dict(self.form)+1
        if varts[0] in self.oper:
            self.read(varts[1], mx, varts[0])
            self.read(varts[2], mx, varts[0])
        elif varts[0] in self.trm.sign.predicates:
            for i in range(1, len(varts)):
                self.read(varts[i], mx, varts[0])
        elif varts[0] in self.kv:
            self.read(varts[1], mx, varts[0])
            self.read(varts[2], mx, varts[0])

    def show(self, ans='', pr=-1, cnt=-1, ind=0):
        if pr ==- 1:
            self.trm.form = self.form
        ans += self.form[(pr, cnt)][ind]
        if self.form[(pr, cnt)][ind] == "¬":
            ans += "("
            ans += self.show('', pr + 1, "¬", 0)
            ans += ")"
        elif self.form[(pr, cnt)][ind] in self.trm.sign.predicates:
            ans += "("
            #pr = find_pls(self.form, self.form[(pr, cnt)][ind], pr)
            for i in range(len(self.form[(pr+1, self.form[(pr, cnt)][ind])])):
                ans += (self.trm.show('', pr + 1, self.form[(pr, cnt)][ind], i) + ", ")
            ans = ans[:-2]
            ans += ")"
        elif self.form[(pr, cnt)][ind] in self.oper:
            ans += ("(" + self.show('', pr + 1, self.form[(pr, cnt)][ind]) + ", " + self.show('', pr + 1, self.form[(pr, cnt)][ind], 1) + ")")
        elif self.form[(pr, cnt)][ind] in self.kv:
            ans += ("(" + self.form[(pr + 1, self.form[(pr, cnt)][ind])][0] + ", " + self.show('', pr + 1, self.form[(pr, cnt)][ind], 1) + ")")
        return ans

    i = -1
    def show1(self, ind = 0):
        ans = ''
        re = 0
        t = mx_in_dict(self.form)
        ans += (self.form[find_pls(self.form, self.i)][ind] + "(")
        self.i += 1
        while self.i < t + 1:
            cur = self.form[find_pls(self.form, self.i)]
            yyy = 0
            for j in cur:
                ans += j
                if j not in self.trm.variables and j not in self.trm.sign.consts:
                    ans += self.show1(yyy)[1:]
                    ans += ", "
                else:
                    ans += ", "
                    re = 1
                yyy+=1
            ans = ans[:-2]
            ans += ")"
            if re:
                return ans
            self.i += 1
        return ans

Trm_example = Formula(["1", "0"], ["p"], [2], ["+", "-"], [2, 2], ["x", "y"])
M = FiniteInterpretation(["1", "0", "2", "3", "4", "5"], ["p"], [2], ["+", "-"], [2, 2])
st1 = [-1]

def eval_trm(variables, work, st):
    t = work.form.copy()
    i = find_pls(t, st[0])
    for j in range(len(t[i])):
        if t[i][j] in work.sign.predicates or t[i][j] in work.sign.functions:
            st[0] += 1
            t[i][j] = eval_trm(variables, work, st)
    if i[0] == -1:
        return t[i][0]
    return (M.count(i[1], t[i], variables))

def eval_for(variables, work, st):
    t = work.form.copy()
    i = find_pls(t, st[0])
    print(i,st[0], "jj")
    print(t[i])
    for j in range(len(t[i])):
        print(t[i][j], t[i], j, "gggovno")
        if t[i][j] in work.trm.sign.predicates or t[i][j] in work.trm.sign.functions or t[i][j] in work.oper or t[i][j] == "¬":
            print(st[0], t[i][j], "kkk")
            st[0] += 1
            t[i][j] = eval_for(variables, work, st)
    if i[0] == -1:
        return t[i][0]
    return (M.count(i[1], t[i], variables))



m = "→(p(+(x, y), -(x, 0)), +(0,1))"
#tr = "p(+(x, x), 1)"
Trm_example.read(m)
print(Trm_example.form)
print(eval_for({"x":1, "y":3}, Trm_example, st1))

