

# Singleton-OC

[TOC]

[文章链接](https://juejin.im/post/5cb067676fb9a0688360f987?utm_source=gold_browser_extension)

### V1.0
```objc
@implementation SingletonClass
+ (instancetype)sharedInstance {
    static SingletonClass *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[self alloc] init];
    });
    return instance;
}
//...
@end

```
弊端：没有保证无论用何种初始化方法， 都应该只有一个实例

### V2.0
```objc
static SingletonClass *sharedInstance = nil;

@implementation SingletonClass
#pragma mark Singleton Methods
+ (id)sharedInstance {
  @synchronized(self) {
      if(sharedInstance == nil)
          sharedInstance = [[super allocWithZone:NULL] init];
  }
  return sharedInstance;
}
+ (id)allocWithZone:(NSZone *)zone {
  return [[self sharedInstance] retain];
}
- (id)copyWithZone:(NSZone *)zone {
  return self;
}
- (id)retain {
  return self;
}
- (unsigned)retainCount {
  return UINT_MAX; //denotes an object that cannot be released
}
- (oneway void)release {
  // never release
}
- (id)autorelease {
  return self;
}
- (void)dealloc {
  // Should never be called, but just here for clarity really.
  [someProperty release];
  [super dealloc];
}
@end

```
弊端：无法实现单例继承

### V3.0
```objc
/// 父类
+ (id)allocWithZone:(NSZone *)zone {
    if (self == SingletonClass.class) {
        return [self sharedInstance];
    }
    return [super allocWithZone:zone];
}

/// 子类
+ (id)allocWithZone:(NSZone *)zone {
    if (self == SingletonClassSon.class) {
        return [self sharedInstance];
    }
    return [super allocWithZone:zone];
}

```
弊端：无法保证初始化方法不可重入。

### V4.0
```objc
// 父类， 子类也类似
static SingletonClass *instance_SingletonClass = nil;
- (instancetype)init {
    static dispatch_semaphore_t semaphore;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        semaphore = dispatch_semaphore_create(1);
    });
    
    SingletonClass *strongRef = instance_SingletonClass;
    dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
    if (strongRef.class != self.class) {
        self = [super init];
        if (self.class == SingletonClass.class) {
            SEL sel = NSSelectorFromString(@"singletonInit");
            if ([self respondsToSelector:sel]) {
                [self performSelector:sel];
            }
            instance_SingletonClass = self;
        }
    }
    dispatch_semaphore_signal(semaphore);
    return self;
}

- (void)singletonInit {
    NSLog(@"caller: %@; SingletonClass customic init", self);
}

```
弊端：不支持 weak 单例

### V5.0
```objc

static SingletonClass *instance_SingletonClass = nil;

+ (id)sharedInstance {
    __block SingletonClass *strongRef = instance_SingletonClass;
    if (strongRef == nil) {
        static dispatch_semaphore_t lock;
        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            lock = dispatch_semaphore_create(1);
        });
        dispatch_semaphore_wait(lock, DISPATCH_TIME_FOREVER);
        if (instance_SingletonClass == nil) {
            strongRef = [[super allocWithZone:NULL] init];
            instance_SingletonClass = strongRef;
        } else {
            strongRef = instance_SingletonClass;
        }
        dispatch_semaphore_signal(lock);
    }
    return strongRef;
}
```
### 使用
```objc
// .h
@interface MyClass : NSObject
AL_AS_SINGLETON; // <- 头文件中加入这个宏

/// your code here ...
@end

// .m
@implementation MyClass
AL_SYNTHESIZE_SINGLETON(MyClass); // <- .m文件中加入这个宏

/// 需要注意的是， 初始化不能直接用 init 方法， 需要用 singletonInit
/// - (void)singletonInit {
///    /// 初始化代码写这里, 比如
///     _myIvar = xxx;
/// }

/// your code here ...
@end
```
